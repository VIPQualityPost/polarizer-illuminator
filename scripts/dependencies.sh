#!/bin/bash
set -e
set -v

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

sudo add-apt-repository --yes ppa:kicad/kicad-8.0-releases
sudo apt update -qq
sudo DEBIAN_FRONTEND=noninteractive apt install --install-recommends kicad kicad-packages3d- kicad-demos-

sudo python3 -m pip install pandas
