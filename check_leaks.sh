#!/bin/bash

valgrind -v --track-origins=yes --leak-check=full --show-leak-kinds=all $1

