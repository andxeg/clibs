#!/bin/bash

valgrind -v --leak-check=full --show-leak-kinds=all $1

