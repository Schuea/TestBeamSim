#!/usr/bin/perl

use strict; # enforce strict checking
use warnings;

my $homeDirectoryPATH="/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/examples/mymarlin"; #TO BE ADJUSTED!!!
my $homeDirectory="MagneticField"; #TO BE ADJUSTED!!!
my $SlicPATH="/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/slic/v03-01-00/build/bin/slic"; #TO BE ADJUSTED!!!
my $ILCSoftPATH="/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/init_ilcsoft.sh"; #TO BE ADJUSTED!!!

my $lcddfile="Pipe_AluWindow_Converter_MAGNET_Collimator_2000gauss.lcdd"; #TO BE ADJUSTED!!!
my $inputfile="Hvalues_for_macros_different_binning_2G.txt";  #TO BE ADJUSTED!!!
my $slciofile="Pipe_AluWindow_Converter_MAGNET_Collimator_2000gauss_"; #TO BE ADJUSTED!!!
my $rootfile="Pipe_AluWindow_Converter_MAGNET_Collimator_2000gauss_";#TO BE ADJUSTED!!!

my $macrofilename="Photonsource_2000gauss_";        
my $jobfilename_="MacroJob_2000gauss";
my $steeringfile_="Sim_Steeringfile_2000gauss";
my $submitfile="SubmitJobs_2000gauss";
my $marlinexe="MarlinExe_2000gauss";
my $slicexe="SlicExe_2000gauss";

my $eventnumberfile="List_of_target_numbers_of_Events";
my $checkexe="Check_numbers_of_Events_2000gauss";

my $macrosDirectory="Macros_2000gauss";
my $jobDirectory="JobsMacro_2000gauss";
my $steeringDirectory="Steeringfiles_2000gauss";
my $slcioDirectory="NEWslcioFiles_2000gauss";
my $ROOTfilesDirectory="rootFiles_2000gauss";

mkdir $macrosDirectory or die "Can't create ",$macrosDirectory,"$!";
mkdir $jobDirectory or die "Can't create ",$jobDirectory,"$!";
mkdir $steeringDirectory or die "Can't create ",$steeringDirectory,"$!";
mkdir $slcioDirectory or die "Can't create ",$slcioDirectory,"$!";
mkdir $ROOTfilesDirectory or die "Can't create ",$ROOTfilesDirectory,"$!";

# configure how many events you want per job
     #my $events_per_job=10000;
#     my $number_of_jobs=49;

#     for (my $i=1; $i<=$number_of_jobs; $i++)
my $l = 1;
my $i = 1;
open(INPUTFILE,$inputfile) or die "Can't open ", $inputfile;
while(<INPUTFILE>)
{
	if($l==1) {next;}
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
	
	if($5==0) {next;}
	
	my $macrofilename=sprintf("%s%5.5i%s",$macrofilename,$i-1,".macro");
	my $lciofilename=sprintf("%s%5.5i%s",$slciofile,$i-1,".slcio");
	
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

	open(EVENTNUMBERFILE,">>$eventnumberfile") or die "Can't open ", $eventnumberfile;
	print EVENTNUMBERFILE $5,"\n"; #Print current number of events
	close(EVENTNUMBERFILE);	

	my $jobfilename=sprintf("%s%i",$jobfilename_,$i-1);
	open(JOBFILE, ">$jobDirectory/$jobfilename") or die "Can't open ", $jobfilename;
	print JOBFILE "#!/bin/bash","\n";
	print JOBFILE $SlicPATH," -m ./",$macrosDirectory,"/",$macrofilename,"\n";
	close(JOBFILE);


	my $steeringfile=sprintf("%s%i%s",$steeringfile_,$i-1,".xml");
	open(STEERINGFILE, ">$steeringDirectory/$steeringfile") or die "Can't open ", $steeringfile;
	print STEERINGFILE "<marlin xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"http://ilcsoft.desy.de/marlin/marlin.xsd\">","\n";
	print STEERINGFILE "<execute>","\n";
	print STEERINGFILE "<processor name=\"Sim_Processor\"/>","\n";
	print STEERINGFILE "</execute>","\n";
	print STEERINGFILE "<global>","\n";
	print STEERINGFILE "<parameter name=\"LCIOInputFiles\"> ",$homeDirectoryPATH,"/",$homeDirectory,"/",$slcioDirectory,"/",$lciofilename," </parameter>","\n";
	print STEERINGFILE "<parameter name=\"MaxRecordNumber\" value=\"200000001\" />","\n";
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
	if($l==2){
		print MARLINEXE "#!/bin/bash","\n";
		print MARLINEXE "export MARLIN_DLL=\$MARLIN_DLL:",$homeDirectoryPATH,"/lib/libSimProcessor.so","\n";
	}
	print MARLINEXE "Marlin ",$steeringDirectory,"/",$steeringfile_,$i-1,".xml","\n";
        print MARLINEXE "mv Sim.root ",$rootfile,$i-1,".root","\n";
        print MARLINEXE "mv ",$rootfile,$i-1,".root ",$ROOTfilesDirectory,"\n";
	close(MARLINEXE);

	open(SLICEXE, ">>$slicexe") or die "Can't open ", $slicexe;
	if($l==2){
		print SLICEXE "#!/bin/bash","\n";
		print SLICEXE "source ",$ILCSoftPATH,"\n";
	}
	print SLICEXE "./",$jobDirectory,"/",$jobfilename,"\n";
	close(SLICEXE);

	open(SUBMITFILE, ">>$submitfile") or die "Can't open ", $submitfile;
	if($l==2){
		print SUBMITFILE "#!/bin/bash","\n";
		print SUBMITFILE "source ",$ILCSoftPATH,"\n";
	}
	print SUBMITFILE "qsub -cwd -o Output/ -e Error/ -l distro=\"sld6\" -q short.q ./",$jobDirectory,"/",$jobfilename,"\n";
	close(SUBMITFILE);

	open(CHECKEXE, ">>$checkexe") or die "Can't open ", $checkexe;
	if($l==2){
		print CHECKEXE "#!/bin/bash","\n";
	}
	print CHECKEXE "lcio_event_counter ",$slcioDirectory,"/",$lciofilename,">> List_of_Event_numbers_2000gauss.txt","\n";
	close(CHECKEXE);
	
	if($l%100==0){
		print "Loop #",$l,"\n";
		print "Still creating files...","\n";
	}
}
continue{
	$l++;
	$i++;
}
close(INPUTFILE);
