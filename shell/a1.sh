#! /bin/bash

/bin/false || { echo "here 1"; date; }
/bin/false || { echo "here 2" && exit 255; }
