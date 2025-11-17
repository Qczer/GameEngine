#!/bin/bash
cd "$(dirname "$(realpath "$0")")/.." || exit 1

./vendor/bin/premake/premake5-linux gmake || {
    echo "Błąd! Naciśnij Enter, aby kontynuować..."
    read
}
