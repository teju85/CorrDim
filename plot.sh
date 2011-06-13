#!/bin/bash
# COMMENTS:
#    Used to draw and display the plots of log(C(R)) v/s log(R)  or
# plot the histogram of the distance matrix.
#
# USAGE:
#    plot.sh [-h, -d, -k, -hist <file>, -cr <file>, -png <pngfile>]
#     -h              Print this help and exit.
#     -d              After generating the plot, display the image.
#     -k              Keep the png file and <file> after displaying.
#     -hist <file>    Consider the input <file> to be the values of
#                     histogram of distance matrix. This must be the
#                     file dumped by the '-dump-dist-hist' option of
#                     'corrdim'.
#     -cr <file>      Consider the input <file> to be the values of
#                     log(CR) v/s log(R). This must the file dumped
#                     by the '-dump' option of 'corrdim'.
#     -png <pngfile>  Name of the output png file to be generated.
#                     This defaults to <file>.png.
#
# NOTE:
#  . Atleast one and exactly one of '-hist' or '-cr' is needed!
#


exeName=$0
function showHelp() {
    head -n 22 $exeName | sed -e 's/^#//' | sed -e 's/!\/bin\/bash//'
    exit $1
}

function createCRPlot() {
    local file=$1
    local img=$2
    echo "set terminal png"
    echo "set output \"$img\""
    echo "set xlabel \"log(R)\""
    echo "set ylabel \"log(C(R))\""
    echo -n "plot \"$file\" using 1:2 title \"log(C(R)) v/s log(R)\" with linespoints,"
    echo "     \"$file\" using 1:3 title \"best-fit\" with linespoints"
}

function createHistPlot() {
    local file=$1
    local img=$2
    echo "set terminal png"
    echo "set output \"$img\""
    echo "set xlabel \"bins\""
    echo "set ylabel \"histogram\""
    echo "set style data histogram"
    echo "plot \"$file\" title \"Distance Matrix Histogram\" with boxes"
}



display="0"
keep="0"
cr="0"
hist="0"
file=""
while [ "$1" != "" ]; do
    case "$1" in
        "-h")
            showHelp 1;;
        "-d")
            display="1"
            shift;;
        "-k")
            keep="1"
            shift;;
        "-hist")
            hist="1"
            shift
            file=$1
            shift;;
        "-cr")
            cr="1"
            shift
            file=$1
            shift;;
        *)
            break;;
    esac
done
plt=".plot_"`date +%s`"_$$.plt"
if [ "$img" = "" ]; then
    img="$file.png"
fi
if [ "$file" = "" ]; then
    echo "You need to pass atleast one of the options '-hist' or '-cr'!"
    exit 1
fi
if [ "$cr" = "1" ] && [ "$hist" = "1" ]; then
    echo "You need to pass only one of the options '-hist' or '-cr'!"
    exit 1
fi
if [ "$cr" = "1" ]; then
    createCRPlot $file $img > $plt
elif [ "$hist" = "1" ]; then
    createHistPlot $file $img > $plt
fi
chmod +x $plt
gnuplot $plt
rm -f $plt
if [ "$display" = "1" ]; then
    display $img
    if [ "$keep" = "0" ]; then
        rm -f $img $file
    fi
fi
