#!/usr/bin/perl

use strict; # enforce strict checking
use warnings;

my $lcddfile="Converter_at1cm.lcdd"; #name and path of the lcdd  file"
my $lciooutputfile_root="Converter_at1cm_"; # name root and path to the output,
#will be changed to nameroot_events_sequence.SLCIO
my $macrofilename_root="Photonsource_";        #macro filename root

my $jobfilename_="MacroJob";
my $steeringfile_="Sim_Steeringfile";
my $inputfile="Hvalues_for_macros.txt";
my $submitfile="SubmitJobs";
my $marlinexe="MarlinExe";

my $macrosDirectory="Macros";
my $jobDirectory="JobsMacro";
my $steeringDirectory="Steeringfiles";
my $slcioDirectory="NEWslcioFiles";


mkdir($macrosDirectory) or die "Can't create ",$macrosDirectory,"$!";
mkdir $jobDirectory or die "Can't create ",$jobDirectory,"$!";
mkdir $steeringDirectory or die "Can't create ",$steeringDirectory,"$!";
mkdir $slcioDirectory or die "Can't create ",$slcioDirectory,"$!";

# configure how many events you want per job
     #my $events_per_job=10000;
#     my $number_of_jobs=49;

#     for (my $i=1; $i<=$number_of_jobs; $i++)
my $i = 1;
open(INPUTFILE,$inputfile) or die "Can't open ", $inputfile;
while(<INPUTFILE>)
{
	if($i==1) {next;}
	$_=~/^(\S+)\s*(\S+)\s*(\S+)\s*(\S+)\s*(\S+)\s*(\S+)/;
  	# =~ means find the specified pattern in $_, the default variable, which currently refers to the line you're on in the file.
	# / is put at the beginning and end of your complete search pattern
	# ^ means at the beginning of the line
	# \ separates different things you're looking for
	# \s matches whitespace (spaces, tabs)
	# \S matches non-whitespace
	# * means zero or more times
	# + means one or more times
	# . matches anything 
	# .* means any character, zero or more times
	# parentheses ( ) put the thing that's matched into a variable, which automatically number themselves, $1, $2, $3... etc.
	
	my $macrofilename=sprintf("%s%5.5i%s",$macrofilename_root,$i-1,".macro");
	my $lciofilename=sprintf("%s%5.5i%s",$lciooutputfile_root,$i-1,".slcio");
	
	#print $slcioDirectory/$lciofilename,"\n";

	#print $macrofilename,"\n";
	open(MACROFILE,">$macrosDirectory/$macrofilename") or die "Can't open ", $macrofilename; #open new macro file
	print MACROFILE "/lcio/PDGFlag true","\n";

	print MACROFILE "/lcio/filename ./",$slcioDirectory,"/",$lciofilename,"\n";
	print MACROFILE "/lcdd/url ./",$lcddfile,"\n";

	print MACROFILE "/run/initialize ","\n";
	print MACROFILE "/generator/select gun","\n";
	print MACROFILE "/gun/particle gamma","\n";
	print MACROFILE "/gun/position ",$2," ",$3," 0","\n";
	print MACROFILE "/gun/direction 0. 0. 1","\n";
	print MACROFILE "/gun/energy ",$6," GeV","\n";
	print MACROFILE "/run/beamOn ",$5,"\n";
	close(MACROFILE);
	

	my $jobfilename=sprintf("%s%i",$jobfilename_,$i-1);
	#print $jobfilename,"\n";
	open(JOBFILE, ">$jobDirectory/$jobfilename") or die "Can't open ", $jobfilename;
	print JOBFILE "#!/bin/bash","\n";
	print JOBFILE "source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh","\n";
	print JOBFILE "localSetupROOT","\n";
	print JOBFILE "localSetupGCC","\n";
	print JOBFILE "source /nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/init_ilcsoft.sh","\n";
	#print JOBFILE "export MARLIN_DLL=\$MARLIN_DLL:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/examples/mymarlin/lib/libSimProcessor.so","\n";
	print JOBFILE "/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/slic/v03-01-00/build/bin/slic -m ./",$macrosDirectory,"/",$macrofilename,"\n";
	#print JOBFILE "Marlin ../",$steeringDirectory,"/Sim_Steeringfile",$i-1,".xml","\n";
	#print JOBFILE "mv Sim.root Sim",$i-1,".root";
	close(JOBFILE);


	my $steeringfile=sprintf("%s%i%s",$steeringfile_,$i-1,".xml");
	#print $steeringfile,"\n";
	open(STEERINGFILE, ">$steeringDirectory/$steeringfile") or die "Can't open ", $steeringfile;
	print STEERINGFILE "<marlin xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"http://ilcsoft.desy.de/marlin/marlin.xsd\">","\n";
	print STEERINGFILE "<execute>","\n";
	print STEERINGFILE "<processor name=\"Sim_Processor\"/>","\n";
	print STEERINGFILE "</execute>","\n";
	print STEERINGFILE "<global>","\n";
	print STEERINGFILE "<parameter name=\"LCIOInputFiles\"> /nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/examples/mymarlin/49MacrosPhotonSource_25M/",$slcioDirectory,"/",$lciofilename," </parameter>","\n";
	print STEERINGFILE "<parameter name=\"MaxRecordNumber\" value=\"12000001\" />","\n";
	print STEERINGFILE "<parameter name=\"SkipNEvents\" value=\"0\" />","\n";
	print STEERINGFILE "<parameter name=\"SupressCheck\" value=\"false\" />","\n";
	print STEERINGFILE "<parameter name=\"GearXMLFile\"> </parameter>","\n";
	print STEERINGFILE "<parameter name=\"Verbosity\" options=\"DEBUG0-4,MESSAGE0-4,WARNING0-4,ERROR0-4,SILENT\"> SILENT  </parameter>","\n";
	print STEERINGFILE "</global>","\n";
	print STEERINGFILE "<processor name=\"Sim_Processor\" type=\"SimProcessor\">","\n";
	print STEERINGFILE "<parameter name=\"InputCollectionName\" type=\"string\" lcioInType=\"MCParticle\"> MCParticle </parameter>","\n";
	print STEERINGFILE "<parameter name=\"InputCollectionName2\" type=\"string\" lcioInType=\"SimCalorimeterHit\"> CarbonFiberHits </parameter>","\n";
	print STEERINGFILE "<parameter name=\"InputCollectionName3\" type=\"string\" lcioInType=\"LCGenericObject\"> MCParticleEndPointEnergy </parameter>","\n";
	print STEERINGFILE "<parameter name=\"OutputCollectionName\" type=\"string\" lcioOutType=\"MCParticle\"> MCParticlePhotonSource </parameter>","\n";
	print STEERINGFILE "</processor>","\n";
	print STEERINGFILE "</marlin>","\n";
	close(STEERINGFILE);

	
	open(MARLINEXE, ">>$marlinexe") or die "Can't open ", $marlinexe;
	if($i==2){
		print MARLINEXE "#!/bin/bash","\n";
		print MARLINEXE "export MARLIN_DLL=\$MARLIN_DLL:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/examples/mymarlin/lib/libSimProcessor.so","\n";
	}
	print MARLINEXE "Marlin 49MacrosPhotonSource_25M/",$steeringDirectory,"/Sim_Steeringfile",$i-1,".xml","\n";
        print MARLINEXE "mv Sim.root Sim",$i-1,".root","\n";
        print MARLINEXE "mv Sim",$i-1,".root rootFiles","\n";
	close(MARLINEXE);

	open(SUBMITFILE, ">>$submitfile") or die "Can't open ", $submitfile;
	print SUBMITFILE "qsub -cwd -o Output/ -e Error/ -q short.q ./",$jobDirectory,"/",$jobfilename,"\n";
	close(SUBMITFILE);
	
	if($i%100==0){
		print "Loop #",$i,"\n";
		print "Still creating files...","\n";
	}
}
continue{
	$i++;
}
close(INPUTFILE);
