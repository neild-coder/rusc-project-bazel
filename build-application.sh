#!/bin/bash

# Color variables
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

root_application="application"
application_lib="rust-application"
output_lib_dir="application/generated-output"

function initialize_base() {
    p_dir=$(pwd)
    cd $root_application
    touch cargo-bazel-lock.json
    cargo check
    cd $p_dir
    CARGO_BAZEL_REPIN=1 bazel sync --only=crate_index
    
    if [ -d $output_lib_dir ]; then
        echo "Build directory exist"
    else
        mkdir -p $output_lib_dir
    fi
}

function rust_build() {
    bazel build //$root_application:$application_lib
    cp bazel-bin/application/librust_application.a $output_lib_dir
}

function application_build() {
    bazel build //$root_application:application
    cp bazel-bin/application/application $output_lib_dir
}

# Function for full build
full_build() {
    echo -e "${GREEN}Starting full build...${NC}"
    initialize_base
    rust_build
    c_build
}

# Function for r build
r_build() {
    echo -e "${GREEN}Starting r build...${NC}"
    rust_build
}

# Function for c build
c_build() {
    echo -e "${GREEN}Starting c build...${NC}"
    application_build
}

# Function for clean
clean() {
    echo -e "${GREEN}Cleaning the build...${NC}"
    rm -rf bazel-*
    cd application
    cargo clean
    rm -rf generated-*
    cd $p_dir
}

# Display the menu options to the user with colors
echo -e "${YELLOW}Choose an option:${NC}"
echo -e "${YELLOW}1. Full Build${NC}"
echo -e "${YELLOW}2. R Build${NC}"
echo -e "${YELLOW}3. C Build${NC}"
echo -e "${YELLOW}4. Clean${NC}"
echo -e "${YELLOW}5. Exit${NC}"

# Read user choice
read -p "Enter your choice [1-5]: " choice

# Handle the user input and call the appropriate function
case $choice in
    1)
        full_build
        ;;
    2)
        r_build
        ;;
    3)
        c_build
        ;;
    4)
        clean
        ;;
    5)
        echo -e "${GREEN}Exiting...${NC}"
        exit 0
        ;;
    *)
        echo -e "${RED}Invalid option. Please choose between 1 and 5.${NC}"
        ;;
esac