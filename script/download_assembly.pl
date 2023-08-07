#!/usr/bin/perl
use strict;
use File::Basename;
use Cwd 'abs_path';
my $bindir = dirname(abs_path(__FILE__));
my $rootdir = dirname($bindir);

print "download PDB\n";
foreach my $line(`cut -f1,2 $rootdir/data/rna.tsv|grep -v '^#'`)
{
    if ($line=~/^(\w+)\t(\w+)/)
    {
        my $pdbid="$1";
        my $chainID="$2";

        my $divided=substr($pdbid,length($pdbid)-3,2);
        my $indir ="$rootdir/pdb/data/assemblies/mmCIF/divided/$divided";
        my $outdir="$rootdir/interim/$divided";
        system("mkdir -p $indir")  if (!-d "$indir");
        system("mkdir -p $outdir") if (!-d "$outdir");
        print "$pdbid\n";
        my $target="$pdbid$chainID";
        my $a=0;
        while (!-s "$outdir/$target.txt")
        {
            $a++;
            if (!-s "$indir/${pdbid}-assembly$a.cif.gz")
            {
                &download_assembly("data/assemblies/mmCIF/divided/$divided/${pdbid}-assembly$a.cif.gz", "$indir/${pdbid}-assembly$a.cif.gz");
                last if (!-s "$indir/${pdbid}-assembly$a.cif.gz");
            }
            print "${pdbid}-assembly$a.cif.gz => $target\n";
            system("cd $outdir; $bindir/cif2pdb $indir/${pdbid}-assembly$a.cif.gz $chainID $target");
        }
    }
}

exit();

sub download_assembly
{
    my ($url_query,$outfile)=@_;
    system("wget -q  ftp://files.wwpdb.org/pub/pdb/$url_query -O $outfile");
    system("wget -q http://files.wwpdb.org/pub/pdb/$url_query -O $outfile") if (!-s "$rootdir/$outfile");
}