#!/bin/bash

cp mods/* ../../kicad-library/mods/
kicad-split --yes -i dc-electronic-load.lib -o ../../kicad-library/libs/
