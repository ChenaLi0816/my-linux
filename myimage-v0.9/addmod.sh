#!/bin/bash
function add_mod() {
    local dep_line=$(modinfo $1 | grep depends)
    local file_line=$(modinfo $1 | grep filename)
    local deps=${dep_line##* }
    for dep in ${deps//,/ }; do
        local dep_l=$dep
        add_mod $dep_l
        echo -e "\e[34m${1}'s depend $dep_l was successfully added\e[0m"
    done
    local path=${file_line##* }
    if [[ ! -e .$path ]]; then
        local dir=${path%/*}
        if [[ ! -d .$dir ]]; then
            mkdir -p .$dir
        fi
        cp -n $path .$path
    fi
}

for mod in $@; do
    add_mod $mod
    echo -e "\e[32m$mod was successfully added\e[0m"
done

