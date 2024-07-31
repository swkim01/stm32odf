# This script should be sourced, not executed.

__realpath() {
    wdir="$PWD"; [ "$PWD" = "/" ] && wdir=""
    arg=$1
    case "$arg" in
        /*) scriptdir="${arg}";;
        *) scriptdir="$wdir/${arg#./}";;
    esac
    scriptdir="${scriptdir%/*}"
    echo "$scriptdir"
}


__verbose() {
    [ -n "${IDF_EXPORT_QUIET}" ] && return
    echo "$@"
}

__main() {
    # The file doesn't have executable permissions, so this shouldn't really happen.
    # Doing this in case someone tries to chmod +x it and execute...

    # shellcheck disable=SC2128,SC2169,SC2039 # ignore array expansion warning
    if [ -n "${BASH_SOURCE-}" ] && [ "${BASH_SOURCE[0]}" = "${0}" ]
    then
        echo "This script should be sourced, not executed:"
        # shellcheck disable=SC2039  # reachable only with bash
        echo ". ${BASH_SOURCE[0]}"
        return 1
    fi

    if [ -z "${STM32ODF_PATH}" ]
    then
        # STM32ODF_PATH not set in the environment.
        # If using bash or zsh, try to guess STM32ODF_PATH from script location.
        self_path=""

        # shellcheck disable=SC2128  # ignore array expansion warning
        if [ -n "${BASH_SOURCE-}" ]
        then
            self_path="${BASH_SOURCE}"
        elif [ -n "${ZSH_VERSION-}" ]
        then
            self_path="${(%):-%x}"
        else
            echo "Could not detect STM32ODF_PATH. Please set it before sourcing this script:"
            echo "  export STM32ODF_PATH=(add path here)"
            return 1
        fi

        # shellcheck disable=SC2169,SC2169,SC2039  # unreachable with 'dash'
        if [[ "$OSTYPE" == "darwin"* ]]; then
            # convert possibly relative path to absolute
            script_dir="$(__realpath "${self_path}")"
            # resolve any ../ references to make the path shorter
            script_dir="$(cd "${script_dir}" || exit 1; pwd)"
        else
            # convert to full path and get the directory name of that
            script_name="$(readlink -f "${self_path}")"
            script_dir="$(dirname "${script_name}")"
        fi
        export STM32ODF_PATH="${script_dir}"
        echo "Setting STM32ODF_PATH to '${STM32ODF_PATH}'"
    else
        # IDF_PATH came from the environment, check if the path is valid
        if [ ! -d "${STM32ODF_PATH}" ]
        then
            echo "STM32ODF_PATH is set to '${STM32ODF_PATH}', but it is not a valid directory."
            echo "If you have set STM32ODF_PATH manually, check if the path is correct."
            return 1
        fi
        # Check if this path looks like an STM32ODF directory
        if [ ! -f "${STM32ODF_PATH}/tools/stm32odf.py" ]
        then
            echo "STM32ODF_PATH is set to '${STM32ODF_PATH}', but it doesn't look like an STM32ODF directory."
            echo "If you have set STM32ODF_PATH manually, check if the path is correct."
            return 1
        fi

        # The varible might have been set (rather than exported), re-export it to be sure
        export STM32ODF_PATH="${STM32ODF_PATH}"
    fi

    old_path="$PATH"

    ODF_ADD_PATHS_EXTRAS="${STM32ODF_PATH}/tools"
    export PATH="${ODF_ADD_PATHS_EXTRAS}:${PATH}"

    if [ -n "$BASH" ]
    then
        path_prefix=${PATH%%${old_path}}
        # shellcheck disable=SC2169,SC2039  # unreachable with 'dash'
        if [ -n "${path_prefix}" ]; then
            __verbose "Added the following directories to PATH:"
        else
            __verbose "All paths are already set."
        fi
        old_ifs="$IFS"
        IFS=":"
        for path_entry in ${path_prefix}
        do
            __verbose "  ${path_entry}"
        done
        IFS="$old_ifs"
        unset old_ifs
    else
        __verbose "Updated PATH variable:"
        __verbose "  ${PATH}"
    fi

    __verbose "Done! You can now compile STM32ODF projects."
    __verbose "Go to the project directory and run:"
    __verbose ""
    __verbose "  stm32odf.py build"
    __verbose ""
}

__cleanup() {
    unset old_path
    unset path_prefix
    unset path_entry
    unset uninstall

    unset __realpath
    unset __main
    unset __verbose
    unset __cleanup

    # Not unsetting IDF_PYTHON_ENV_PATH, it can be used by IDF build system
    # to check whether we are using a private Python environment

    return $1
}

__main
__cleanup $?
