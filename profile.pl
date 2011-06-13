#!/usr/bin/env perl
#
# Script to profile the runtime and memory usage of 'corrdim'
#

use strict;
use warnings;

sub runAtest {
    my ($numele, $lowmem) = @_;
    my $cmd = "./corrdim -numele $numele";
    $cmd .= " -lowmem"   if($lowmem);
    printf("Working on '$cmd'... ");
    my @output = split(/\n/, `$cmd`);
    my $memory = -1;
    my $time = -1;
    foreach my $line (@output) {
	if($line =~ /Maximum memory usage/) {
	    $memory = (split(/\s+/, $line))[5];
	    $memory =~ s/\~//;
	    next;
	}
	if($line =~ /Total time taken/) {
	    $time = (split(/\s+/, $line))[3];
	    next;
	}
    }
    printf(" (Mem=$memory Time=$time)\n");
    return ($memory, $time);
}



# profile
if((scalar(@ARGV) != 1) || ($ARGV[0] ne "running_from_Makefile")) {
    die "You cannot run this script from outside 'Makefile'!";
}
my @elements = (2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000);
my $profile = "results_profile.txt";
my $png_time = "results_time.png";
my $png_mem = "results_mem.png";
my $plt = ".results.plt";
open(FILE, ">$profile") or die("Failed to open '$profile'!");
foreach my $numele (@elements) {
    my ($mem1, $time1) = runAtest($numele, 0);
    my ($mem2, $time2) = runAtest($numele, 1);
    printf FILE "%6d %.6f %4d %.6f %4d\n", $numele, $time1, $mem1, $time2, $mem2;
}
close(FILE);
# plot
print "Generating the plot...\n";
open(PLT, ">$plt") or die("Failed to open '$plt'!");
print PLT "set terminal png\n";
print PLT "set output \"$png_time\"\n";
print PLT "set xlabel \"Num Elements\"\n";
print PLT "set ylabel \"Time(s)\"\n";
print PLT "plot \"$profile\" using 1:2 title \"CorrDimTime\" with linespoints,";
print PLT "     \"$profile\" using 1:4 title \"CorrDimLowMemTime\" with linespoints\n";
print PLT "\n";
print PLT "set output \"$png_mem\"\n";
print PLT "set xlabel \"Num Elements\"\n";
print PLT "set ylabel \"Memory(MB)\"\n";
print PLT "plot \"$profile\" using 1:3 title \"CorrDimMem\" with linespoints,";
print PLT "     \"$profile\" using 1:5 title \"CorrDimLowMemMem\" with linespoints\n";
close(PLT);
system("gnuplot $plt");
# clean
print "Cleaning...\n";
unlink($plt);
print "Done...\n";
