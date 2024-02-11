#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import os
import argparse
from datetime import datetime
import pathlib
import multiprocessing as mp
import multiprocessing
import xml.etree.ElementTree as ET
from xml.dom.minidom import parseString
from itertools import product
import json


CRED = ""
HEADER = '\033[95m'
OKBLUE = '\033[94m'
OKCYAN = '\033[96m'
OKGREEN = '\033[92m'
WARNING = '\033[93m'
FAIL = '\033[91m'
ENDC = '\033[0m'
BOLD = '\033[1m'
UNDERLINE = '\033[4m'

######################### my used
CPATH = "\033[0;94m"
CFILE = "\033[96m"
CHEADER = '\033[95m'
CGLOBALVAR = "\033[96m"
CINFO = '\033[92m'
BOLD = '\033[1m'
CWARNING = '\033[93m'
CERROR = '\033[31m'
ENDC = '\033[0m'
CPYEXECUTOR = '\033[92m'
#########################

####################################################################################################
#################################### Properties to change ##########################################
####################################################################################################
###*** Paths names setup ***###
HEP_SOFT_DIR_NAME = "HEP-soft"
USER_ROOT_DIR_NAME = "/t2k/users/shvartsman"

### variant 1, default ###
# T2K_ND280_UPGRADE_SOFT_SRC_DIRNAME = "t2k-nd280-upgrade-copied-node204"
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-copied-node204-build-01"  # worked
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-copied-node204-build-03"
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-copied-node204-build-04"
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-copied-node204-build-02"
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-copied-node204-build-05"  # test new tech for segfault in neutrinos
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-copied-node204-build-main"
### variant 1, default ###

### !!! ###
T2K_ND280_UPGRADE_SOFT_SRC_DIRNAME = "t2k-nd280-upgrade-git"
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-git-node204-build-01"  # works, store only first and last poits of tracks
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-git-node204-build-04"  # works, really? (build: store not all pts, each pt has additional "MaterialName" and "ProcessName")
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-git-node204-build-05"  # (build: store not all pts, each pt has additional "MaterialName" and "ProcessName")
# T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-git-node204-build-06"  # (build: store not all pts, each pt has additional "MaterialName" and "PreProcessName" and "PostProcessName")
T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = "t2k-nd280-upgrade-git-node204-build-07"  # like "t2k-nd280-upgrade-git-node204-build-06", but with event vertices generated uniformly in SFGD
### !!! ###

###*** Needed scripts names setup ***###
SUBMIT_SCRIPT = 'submit_nd280upgrade_sim-my'
CONFIG_FILE = "configuration_cluster_SuperFGD-v14.02.2021.xml"
README_SIM_FILE_NAME = "README.html"
# README_SIM_FILE_NAME = "README.md"

###*** ROOT CERN ***###
ROOT_CERN_SRC_DIR_NAME = "root"
# ROOT_CERN_BLD_DIR_NAME = "root-build-node204-00"  // ------------------------------------------------------------------------ //

ROOT_CERN_INS_DIR_NAME = "root-install-node204-00"

###*** GEANT4 ***###
GEANT4_BASE_DIRNAME = "/opt/geant4"

###*** Genie filename ***###
GENIE_NU_SIM_FILENAME = ""

###*** Workdir location ***###
WORK_DIRNAME = "T2KND280soft_/node204-new-scheme"

###*** Simulation output directories names ***###
SIM_DIRNAME = "sim"
####################################################################################################

####################################################################################################
######################################## GLOBAL VARIABLES ##########################################
####################################################################################################
USER_ROOT_PATH = pathlib.Path(USER_ROOT_DIR_NAME)                                                  #
HEP_SOFT_PATH = USER_ROOT_PATH / HEP_SOFT_DIR_NAME                                                 #
T2K_ND280_UPGRADE_SOFT_BLD_PATH = HEP_SOFT_PATH / T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME               #
T2K_ND280_UPGRADE_SOFT_SRC_PATH = HEP_SOFT_PATH / T2K_ND280_UPGRADE_SOFT_SRC_DIRNAME               #
ND280UPEVENT_DICTS_PATH = f'{T2K_ND280_UPGRADE_SOFT_SRC_PATH}/src/app/nd280UpEvent'                #
# ND280UPEVENT_DICTS_DIR = f'{T2K_ND280_UPGRADE_SOFT_SRC_PATH}/src/app/nd280UpEvent'  # worked

### test new
# T2K_ND280_UPGRADE_SOFT_SRC_PATH = "/t2k/users/shvartsman/HEP-soft/t2k-nd280-upgrade-git"
# ND280UPEVENT_DICTS_DIR = f'{T2K_ND280_UPGRADE_SOFT_SRC_PATH}/src/app/nd280UpEvent'
### test new

# WORK_PATH = USER_ROOT_PATH / "T2KND280soft_" / "node204-new-scheme"
WORK_PATH = USER_ROOT_PATH / WORK_DIRNAME                                                          #
SIM_PATH = WORK_PATH / SIM_DIRNAME                                                                 #
GENIE_NU_SIM_FILE_PATH = ""  # now unused
SUBMIT_SCRIPT_PATH = SIM_PATH / SUBMIT_SCRIPT                                                      #
CONFIG_FILE_PATH = SIM_PATH / CONFIG_FILE                                                          #
README_SIM_FILE_PATH = SIM_PATH / README_SIM_FILE_NAME                                             #

ROOT_CERN_SRC_PATH = HEP_SOFT_PATH / ROOT_CERN_SRC_DIR_NAME
# ROOT_CERN_BLD_PATH = HEP_SOFT_PATH / ROOT_CERN_BLD_DIR_NAME
ROOT_CERN_INS_PATH = HEP_SOFT_PATH / ROOT_CERN_INS_DIR_NAME                                        #

GEANT4_PATH = pathlib.Path(GEANT4_BASE_DIRNAME)                                                    #

CURRENT_SCRIPT_PATH = pathlib.Path.cwd()                                                           #
####################################################################################################

### such a strange form is neede for generating xml in appropriate form for t2k nd280 up soft
SIM_DEFAULT_CONFIG = {
    "StoreGeometry": "0",
    "IsRandomSeed": "1",
    "Generator": "GENIE",
    "Path2file": "/home/shvartsman/diploma/AnalisysResults/",
    "filename": "GENIE_geometry_SuperFGD_1e21_numu.root",
    "treename": "gRooTracker",
    "forwTPCdefault": "false",
    "forwTPCPos1_X": "0",
    "forwTPCPos1_Y": "0",
    "forwTPCPos1_Z": "-211",
    "forwTPCPos2_X": "0",
    "forwTPCPos2_Y": "0",
    "forwTPCPos2_Z": "+1066",
    "forwTPCPos3_X": "0",
    "forwTPCPos3_Y": "0",
    "forwTPCPos3_Z": "+2343",
    "Targetdefault1": "false",
    "TargetMaterial1": "FGDScintillator",
    "Targetlength1": "1920",
    "Targetwidth1": "1920",
    "Targetheight1": "560",
    "TargetPos1_X": "0",
    "TargetPos1_Y": "0",
    "TargetPos1_Z": "-1760.412",
    "Targetdefault2": "false",
    "TargetMaterial2": "WAGASCIScintillatorEmpty",
    "Targetlength2": "1300",
    "Targetwidth2": "1864",
    "Targetheight2": "600",
    "TargetPos2_X": "0",
    "TargetPos2_Y": "0",
    "TargetPos2_Z": "+1206",
    "FGDdefault1": "false",
    "FGDMaterial1": "FGDScintillator",
    "FGDlength1": "303",
    "FGDwidth1": "1864.3",
    "FGDheight1": "1864.3",
    "FGDPos1_X": "0",
    "FGDPos1_Y": "0",
    "FGDPos1_Z": "+427.5",
    "FGDdefault2": "false",
    "FGDMaterial2": "FGD2Uniform",
    "FGDlength2": "303",
    "FGDwidth2": "1864.3",
    "FGDheight2": "1864.3",
    "FGDPos2_X": "0",
    "FGDPos2_Y": "0",
    "FGDPos2_Z": "+1704.5",
    "SuperFGDCubeEdge1": "10",
    "SuperFGDCubeNum1_X": "192",
    "SuperFGDCubeNum1_Y": "56",
    "SuperFGDCubeNum1_Z": "184",
    "SuperFGDPos1_X": "0",
    "SuperFGDPos1_Y": "16",
    "SuperFGDPos1_Z": "0.",
    "CFBoxCFRPThickness": "2",
    "CFBoxAIREXThickness": "16",
    "CFBoxPlasticThickness": "4",
    "CFBoxInnerClearanceThickness": "5",
    "CFBoxOuterClearanceThickness": "5",
    "FlatCableThickness": "0.101",
    "FlatCableN": "6",
    "SuperFGDCubeEdge2": "10",
    "SuperFGDCubeNum2_X": "96",
    "SuperFGDCubeNum2_Y": "56",
    "SuperFGDCubeNum2_Z": "192",
    "SuperFGDPos2_X": "480.",
    "SuperFGDPos2_Y": "0",
    "SuperFGDPos2_Z": "-1695",
    "HATPCFCName": "FC",
    "HATPCUpWidth": "2200",
    "HATPCUpHeight": "840",
    "HATPCUpLength": "1840",
    "HATPCUpDriftWidth": "1700",
    "HATPCUpDriftHeight": "700",
    "HATPCUpDriftLength": "1700",
    "HATPCUpCathodeThickness": "13.2",
    "HATPCUpInnerBoxWall": "100",
    "HATPCUpOuterBoxWall": "100",
    "HATPCUpSteppingLimit": "1.",
    "ActiveHATPCUpVerticalOffset": "25",
    "HATPCUpPos_X": "0",
    "HATPCUpPos_Y": "734.676",
    "HATPCUpPos_Z": "-1760.412",
    "HATPCDownWidth": "2200",
    "HATPCDownHeight": "840",
    "HATPCDownLength": "1840",
    "HATPCDownDriftWidth": "1700",
    "HATPCDownDriftHeight": "700",
    "HATPCDownDriftLength": "1700",
    "HATPCDownCathodeThickness": "13.2",
    "HATPCDownCO2Top": "100",
    "HATPCDownCO2Bottom": "100",
    "HATPCDownInnerBoxWall": "100",
    "HATPCDownOuterBoxWall": "100",
    "HATPCDownSteppingLimit": "1.",
    "ActiveHATPCDownVerticalOffset": "25",
    "HATPCDownPos_X": "0",
    "HATPCDownPos_Y": "-732.5",
    "HATPCDownPos_Z": "-1760.412",
    "FGD3Dlength1": "100",
    "FGD3Dwidth1": "600",
    "FGD3Dheight1": "600",
    "FGD3DPos1_X": "0",
    "FGD3DPos1_Y": "0",
    "FGD3DPos1_Z": "0",
    "FGD3Dlength2": "13",
    "FGD3Dwidth2": "18",
    "FGD3Dheight2": "6",
    "FGD3DPos2_X": "0",
    "FGD3DPos2_Y": "0",
    "FGD3DPos2_Z": "0",
    "FGDlikeBarEdge": "10",
    "FGDlikeNum_AlongX": "130",
    "FGDlikeNum_AlongZ": "180",
    "FGDlikeNum_Layer": "30",
    "FGDlikePos_X": "0",
    "FGDlikePos_Y": "0",
    "FGDlikePos_Z": "0",
    "ToFdefault_TopUp": "false",
    "ToFPlaneXYNum_TopUp": "21",
    "ToFBarwidth_TopUp": "10",
    "ToFBarheight_TopUp": "120",
    "ToFBarlength_TopUp": "2085",
    "ToFRotX_TopUp": "90",
    "ToFRotY_TopUp": "90",
    "ToFRotZ_TopUp": "0",
    "ToFPosX_TopUp": "0",
    "ToFPosY_TopUp": "+1245",
    "ToFPosZ_TopUp": "-1760.412",
    "ToFdefault_BotUp": "false",
    "ToFPlaneXYNum_BotUp": "21",
    "ToFBarwidth_BotUp": "10",
    "ToFBarheight_BotUp": "120",
    "ToFBarlength_BotUp": "2085",
    "ToFRotX_BotUp": "90",
    "ToFRotY_BotUp": "90",
    "ToFRotZ_BotUp": "0",
    "ToFPosX_BotUp": "0",
    "ToFPosY_BotUp": "-1265",
    "ToFPosZ_BotUp": "-1760.412",
    "ToFdefault_RightUp": "false",
    "ToFPlaneXYNum_RightUp": "20",
    "ToFBarwidth_RightUp": "10",
    "ToFBarheight_RightUp": "120",
    "ToFBarlength_RightUp": "2085",
    "ToFRotX_RightUp": "0",
    "ToFRotY_RightUp": "90",
    "ToFRotZ_RightUp": "0",
    "ToFPosX_RightUp": "-1270",
    "ToFPosY_RightUp": "0",
    "ToFPosZ_RightUp": "-1760.412",
    "ToFdefault_LeftUp": "false",
    "ToFPlaneXYNum_LeftUp": "20",
    "ToFBarwidth_LeftUp": "10",
    "ToFBarheight_LeftUp": "120",
    "ToFBarlength_LeftUp": "2085",
    "ToFRotX_LeftUp": "0",
    "ToFRotY_LeftUp": "90",
    "ToFRotZ_LeftUp": "0",
    "ToFPosX_LeftUp": "+1270",
    "ToFPosY_LeftUp": "0",
    "ToFPosZ_LeftUp": "-1760.412",
    "ToFdefault_BackUp": "false",
    "ToFPlaneXYNum_BackUp": "20",
    "ToFBarwidth_BackUp": "10",
    "ToFBarheight_BackUp": "120",
    "ToFBarlength_BackUp": "2300",
    "ToFRotX_BackUp": "0",
    "ToFRotY_BackUp": "0",
    "ToFRotZ_BackUp": "0",
    "ToFPosX_BackUp": "0",
    "ToFPosY_BackUp": "0",
    "ToFPosZ_BackUp": "-2800",
    "ToFdefault_FrontUp": "false",
    "ToFPlaneXYNum_FrontUp": "20",
    "ToFBarwidth_FrontUp": "10",
    "ToFBarheight_FrontUp": "120",
    "ToFBarlength_FrontUp": "2300",
    "ToFRotX_FrontUp": "0",
    "ToFRotY_FrontUp": "0",
    "ToFRotZ_FrontUp": "0",
    "ToFPosX_FrontUp": "0",
    "ToFPosY_FrontUp": "0",
    "ToFPosZ_FrontUp": "-723",
    "DegraderLength": "230",
    "DegraderWidth": "240",
    "DegraderHeight": "80",
    "DegraderPositionX": "0",
    "DegraderPositionY": "16",
    "DegraderPositionZ": "-355",
    "UseHATPCUp": "false",
    "UseHATPCDown": "false",
    "UseForwTPC1": "false",
    "UseForwTPC2": "false",
    "UseForwTPC3": "false",
    "UseTarget1": "false",
    "UseTarget2": "false",
    "UseFGD1": "false",
    "UseFGD2": "false",
    "UseSuperFGD1": "true",
    "UseSuperFGD2": "false",
    "UseCFBox": "true",
    "UsePCB": "true",
    "UseFlatCable": "true",
    "UseFGD3D1": "false",
    "UseFGD3D2": "false",
    "UseFGDlike": "false",
    "UseDegrader": "false",
    "UseToF_TopUp": "false",
    "UseToF_BotUp": "false",
    "UseToF_RightUp": "false",
    "UseToF_LeftUp": "false",
    "UseToF_BackUp": "false",
    "UseToF_FrontUp": "false",
    "UseMagnet": "false",
    "UseSMRD": "true",
    "UseBasket": "false",
    "UseDsECal": "false",
    "UseBrlECal": "false",
    "UseP0DECal": "false",
    "UseP0D": "false",
    "InvisForwTPC1": "false",
    "InvisForwTPC2": "false",
    "InvisForwTPC3": "false",
    "InvisTarget1": "false",
    "InvisTarget2": "false",
    "InvisFGD1": "false",
    "InvisFGD2": "false",
    "InvisSuperFGD": "true",
    "InvisFGD3D": "true",
    "InvisFGDlike": "true",
    "InvisToF_TopUp": "false",
    "InvisToF_BotUp": "false",
    "InvisToF_RightUp": "false",
    "InvisToF_LeftUp": "false",
    "InvisToF_BackUp": "false",
    "InvisToF_FrontUp": "false",
    "InvisMagnet": "true",
    "InvisBasket": "true",
    "InvisSMRD": "false",
    "InvisDsECal": "false",
    "InvisBrlECal": "false",
    "InvisP0DECal": "false",
    "InvisP0D": "false",
}

SUBMIT_SCRIPT_CONTENT = """#!/usr/bin/perl


#-------------------------------------------------------------------------------------------------------------------------------
# Submit a GEANT4 simulation job
#
# Inputs
#  --resource               : Computer resource. Default: interactive.
#  --tag                    : Tag for output files.
#  --config-file            : Analysis configuration file from the config/ directory.
#
#  --cpu-time               : Required CPU time
#  --job-dir                : Directory for job scripts and outputs. Default: `pwd`.
#  --softw-dir              : code top-level directory.
# D.Sgalaberna
#-------------------------------------------------------------------------------------------------------------------------------
#


use File::Path;

$iarg=0;
foreach (@ARGV) {

    if($iarg % 2 == 1) {$iarg++; next;}

    ##################################### Common ########################################
    if   ($_ eq '--resource'         ) { $resource        = $ARGV[$iarg+1]; }
    elsif($_ eq '--cpu-time'         ) { $cput            = $ARGV[$iarg+1]; }
    elsif($_ eq '--tag'              ) { $tag             = $ARGV[$iarg+1]; }
    elsif($_ eq '--nruns'            ) { $nruns           = $ARGV[$iarg+1]; }
    elsif($_ eq '--config'           ) { $config_file     = $ARGV[$iarg+1]; }
    elsif($_ eq '--vis-file'         ) { $vis_file        = $ARGV[$iarg+1]; }
    elsif($_ eq '--first-expt'       ) { $first_expt      = $ARGV[$iarg+1]; }
    elsif($_ eq '--nexpts'           ) { $nexpts          = $ARGV[$iarg+1]; }
    elsif($_ eq '--job-dir'          ) { $job_dir         = $ARGV[$iarg+1]; }
    elsif($_ eq '--softw-dir'        ) { $softw_dir       = $ARGV[$iarg+1]; }
    elsif($_ eq '--exe-clust-dir'    ) { $exe_clust_dir   = $ARGV[$iarg+1]; }
    elsif($_ eq '--exe-loc-dir'      ) { $exe_loc_dir     = $ARGV[$iarg+1]; }

    else {
    print "Unrecognised argument ".$_."\\n"; exit;
    }
    $iarg++;
}

$exe_clust_dir      = "bin/"                        unless defined $exe_clust_dir; # path starting from $softw_dir
$exe_loc_dir        = "build/app/EffStudy"          unless defined $exe_loc_dir; # path starting from $softw_dir
$resource           = "interactive"                 unless defined $resource;
$tag                = "test"                        unless defined $tag;
$config_file        = "$softw_dir/src/app/EffStudy/config/configuration_cluster_ref_sfgdonly.xml" unless defined $config_file;
$vis_file           = "$softw_dir/src/app/EffStudy/mac/vis_pgun.mac"          unless defined $vis_file;
$nruns              = 1                             unless defined $nruns;      # number of jobs
$nexpts             = 100                           unless defined $nexpts;     # number of events per job
$first_expt         = 0                             unless defined $first_expt; # first event
$queue              = "sx"                          unless defined $queue;

if($resource eq "parallel"){
    open(par_file, ">$job_dir/commands_sim.sh") or die("Can not create the sbatch script");
    print par_file "#!/bin/bash -l  \\n";
}

$irun = 0;
while($irun < $nruns) { # loop over the jobs

    $evt_first = $first_expt + $irun * $nexpts;

    # $arguments = "$softw_dir/$exe_loc_dir/EffStudyApp $vis_file $config_file $job_dir/$tag-Exp$evt_first-Nexpt$nexpts $evt_first $nexpts";  #!!!
    $arguments = "$exe_loc_dir/EffStudyApp $vis_file $config_file $job_dir/$tag-Exp$evt_first-Nexpt$nexpts $evt_first $nexpts";  #!!!

    $command = "$arguments";
    # $command .= "; mv EffStudy_0.root EffStudy_";
    # $file_name = $irun + 1;
    # $command .= "$file_name.root";

    print "\\n";
    print "Job $irun \\n";
    print "First event: $evt_first \\n";
    print "# of events: $nexpts \\n";
    print "$command \\n";
    print "\\n////////////////////////////// \\n\\n";

    ################################### Submit the job ##################################

    #
    # UNIGE (Atlas cluster), $job_dir not used, $tag gives path/name of output file
    #
    if($resource eq "UNIGE-Atlas") {

    $sbatchfile = "$job_dir/$tag-$evt_first-$nexpts.sbatch";
    $outfiledir = "$job_dir/";
    $errfiledir = "$job_dir/";
    #open(PBS, ">$job_dir/$tag-$evt_first-$nexpts.sbatch") or die("Can not create the sbatch script");
    open(PBS, ">$sbatchfile") or die("Can not create the sbatch script");
    print PBS "#!/bin/bash -l  \\n";
    print PBS "source $softw_dir/T2KND280Up_CLHEP.sh $softw_dir \\n";
    print PBS "\\n";
    print PBS "env \\n";
    print PBS "\\n";
    print PBS "uname -n \\n";
    print PBS "\\n";
    print PBS "$command \\n";
    close(PBS);
    `qsub -q $queue -l mem=6000mb -l vmem=6000mb $sbatchfile`;
    }

    if($resource eq "UNIGE-Atlas-Slurm") {

    #print "Use SLURM \\n";

    #$queue = "rhel6-$queue"; # Correct SLURM queue, rhel-short, not short!
    #print "$queue \\n";

    $sbatchfile = "$job_dir/$tag-$evt_first-$nexpts.sbatch";
    $outfile = "$job_dir/$tag-$evt_first-$nexpts-%j.out";
    $errfile = "$job_dir/$tag-$evt_first-$nexpts-%j.err";
    #open(PBS, ">$job_dir/$tag-$evt_first-$nexpts.sbatch") or die("Can not create the sbatch script");
    open(PBS, ">$sbatchfile") or die("Can not create the sbatch script");
    print PBS "#!/bin/bash -l  \\n";
    print PBS "#SBATCH --time=00:25:00 \\n";

    #print PBS "#SBATCH --partition=$queue --mem-per-cpu=6000  \\n"; # in MB. Same as dphys_largemem default RAM
    print PBS "#SBATCH --partition=$queue  \\n"; # in MB. Same as dphys_largemem default RAM
    print PBS "#SBATCH --mem-per-cpu=6000  \\n"; # in MB. Same as dphys_largemem default RAM

    print PBS "#SBATCH --nodes=1  \\n";
    print PBS "#SBATCH --ntasks-per-node=1  \\n";
    print PBS "#SBATCH --output=$outfile  \\n";
    print PBS "#SBATCH --error=$errfile  \\n";
    #print PBS "#SBATCH --jobname=myJob \\n";
    #print PBS "echo \\$SLURM_JOB_NODELIST \\n"; # print the node  number
    print PBS "source $softw_dir/T2KND280Up_CLHEP.sh $softw_dir \\n";
    print PBS "$command \\n";
    close(PBS);
    `sbatch $sbatchfile`;
    #`sbatch -t 07:00:00 $sbatchfile`;
    }

    if($resource eq "KEKCC") {
    $sbatchfile = "$job_dir/$tag-$evt_first-$nexpts.sbatch";
    open(PBS, ">$sbatchfile") or die("Can not create the sbatch script");
    print PBS "#!/bin/csh \\n";
    print PBS "cd $job_dir  \\n";
    print PBS "source T2KND280Up_CLHEP.csh \\n";
    print PBS "cd $job_dir  \\n";
    print PBS "\\n";
    print PBS "env \\n";
    print PBS "\\n";
    print PBS "uname -n \\n";
    print PBS "\\n";
    print PBS "$command \\n";
    close(PBS);
    `chmod 765 $sbatchfile`;
    `bsub -q $queue $sbatchfile`;
    }

    if($resource eq "interactive") {
        print "$command";
            system($command);
    }

    if($resource eq "parallel") {
        print par_file "#!/bin/bash -l  \\n";
        print par_file "$command; echo \\"job $irun from $nruns done\\" \\n";
        print "Parallel\\n";
        print "$command\\n";
    }




    $irun=$irun+1;
} # end loop over jobs

if($resource eq "parallel") {
    close(par_file);
    $run_command = "nice -n19 parallel -j 15 -a $job_dir/commands_sim.sh";
    print "running parallel \\n";
    system($run_command);
    unlink "$job_dir/commands_sim.sh";
}
"""


def create_simulation_directories(job_dir_path):
    vis_path = job_dir_path / "vis-files"
    sim_path = job_dir_path / 'root-sim'
    anr_path = job_dir_path / 'an-res'

    job_dir_path.mkdir(parents=True, exist_ok=True)
    vis_path.mkdir(parents=True, exist_ok=True)
    sim_path.mkdir(parents=True, exist_ok=True)
    anr_path.mkdir(parents=True, exist_ok=True)

    print(f"\n{CINFO}Directories:\n"
          f"    {CPATH}{job_dir_path}{ENDC}\n"
          f"    {CPATH}{vis_path}{ENDC}\n"
          f"    {CPATH}{sim_path}{ENDC}\n"
          f"    {CPATH}{anr_path}{ENDC}\n"
          f"  {CINFO}are created{ENDC}\n")

    return vis_path, sim_path, anr_path


def create_parallel_commands_file_v2(job_folder_path, vis_path, sim_path, nexpts):
    parallel_commands_file_path = job_folder_path / "parallel-commands.txt"
    vis_files_lst = vis_path.glob("*.mac")

    str2write_p1 = (
        f'{SUBMIT_SCRIPT_PATH} '
        f'--softw-dir {T2K_ND280_UPGRADE_SOFT_SRC_PATH} '
        f'--exe-loc-dir {T2K_ND280_UPGRADE_SOFT_BLD_PATH}/app/EffStudy '
        f'--job-dir {sim_path} '
        '--resource interactive '
        f'--nexpts {nexpts} --nruns 1 '
        f'--config {CONFIG_FILE_PATH} ')

    with parallel_commands_file_path.open('w') as f:
        for filename in vis_files_lst:
            vis_file = vis_path / filename
            filename = filename.name
            vis_file = f'--vis-file "{vis_file}" ' +\
                f'--tag {filename[:-8]}_n{nexpts} > {sim_path}/log-{filename[:-8]}_n{nexpts}.log \n'
            f.write(str2write_p1 + vis_file)

    print(f'{CINFO}File "{CFILE}{parallel_commands_file_path.name}'
          f'{CINFO}" at "{CPATH}{parallel_commands_file_path.parent}'
          f'{CINFO}" is created{ENDC}')


def create_parallel_commands_file_v3(job_folder_path, vis_path, sim_path, nexpts):
    parallel_commands_file_path = job_folder_path / "parallel-commands.txt"
    vis_files_lst = vis_path.glob("*.mac")

    njobs = 0
    str2write = ""
    for filename in vis_files_lst:
        vis_file = vis_path / filename
        filename = filename.name
        tag = f"{filename[:-8]}_n{nexpts}"
        _str2write = (
            f"{T2K_ND280_UPGRADE_SOFT_BLD_PATH}/app/EffStudy/EffStudyApp "
            f"{vis_file} {CONFIG_FILE_PATH} {sim_path}/{tag}-Exp0-Nexpt{nexpts} 0 {nexpts} "
            f"> {sim_path}/log-{tag}.log\n")
        str2write += _str2write
        njobs += 1

    with parallel_commands_file_path.open('w') as f:
        f.write(str2write)

    print(f'{CINFO}File "{CFILE}{parallel_commands_file_path.name}'
          f'{CINFO}" at "{CPATH}{parallel_commands_file_path.parent}'
          f'{CINFO}" is created{ENDC}')
    return parallel_commands_file_path, njobs


def generate_config_xml(file_path, config):
    if (not set(config.keys()).issubset(set(SIM_DEFAULT_CONFIG.keys())) and config):
        raise NameError(f"{CERROR}config options are incosistent{ENDC}")

    file_structure = ET.Element('fileStructure')
    main_packg = ET.SubElement(file_structure, 'Main_Package')
    main_packg.text = "File_Navigate"
    dep_det = ET.SubElement(file_structure, 'Dependency_Details')
    dep = ET.SubElement(dep_det, 'Dependency')

    for key, val in SIM_DEFAULT_CONFIG.items():
        item = ET.SubElement(dep, key)
        text = "\n" + config[key] + "\n      " if key in config else "\n" + val + "\n      "
        item.text = text

    fs = ET.tostring(file_structure, encoding="utf-8")

    dom = parseString(fs)
    dom_str = dom.toprettyxml(indent="  ")
    with file_path.open("w") as f:
        f.write(dom_str)

    msg = (f"{CINFO}Created detector configuration xml file "
           f"\"{CFILE}{file_path.name}{ENDC}\""
           f"{CINFO} at "
           f"\"{CPATH}{file_path.parent}{ENDC}\"")
    print(msg)


def create_configuration_file(file_path, config_file_content=SUBMIT_SCRIPT_CONTENT):
    with file_path.open("w") as f:
        f.write(config_file_content)

    msg = (f"{CINFO}Created simulation configuration file "
           f"\"{CFILE}{file_path.name}{ENDC}\""
           f"{CINFO} at "
           f"\"{CPATH}{file_path.parent}{ENDC}\"")
    print(msg)


def html_colored_text(text, html_color):
    return f"<span style=\"color:{html_color}\">{text}</span>"


def write_readme_file(particles, magnetic_field_list, momentum_vector_list, kinetic_energy_list, gun_position_list,
                      job_folder_path, build_dir_path, nexpts, creation_time):
    # prepare information string
    hct = html_colored_text
    magnetic_filed_str_info = "".join(
        f'({hct(f"{mf[0]} {mf[1]} {mf[2]}", "#00FF00")} {hct(mf[3], "#22AAFF")})'
        for mf in magnetic_field_list)
    momentum_vector_str_info = "".join(
        f'({hct(f"{mv[0]} {mv[1]} {mv[2]}", "#00FF00")} {hct(mv[3], "#22AAFF")})'
        for mv in momentum_vector_list) if momentum_vector_list else ""
    kinetic_energy_str_info = "".join(
        f'({hct(ke[0], "#00FF00")} {hct(ke[1], "#22AAFF")})'
        for ke in kinetic_energy_list) if kinetic_energy_list else ""
    gun_position_str_info = "".join(
        f'({hct(f"{gp[0]} {gp[1]} {gp[2]}", "#00FF00")} {hct(gp[3], "#22AAFF")})'
        for gp in gun_position_list)
    readme_info_string = (
        f"<div class=simulation>\n"
        f"  <H3>Simulation at <span style=\"color:red;\">{creation_time.strftime('%d.%m.%Y %H:%M:%S')}</span></H3>\n"

        f"  <div class=setup-parameters>\n"

        f"    <H4>setup parameters: names</H4>\n"
        f"    &#x27F6; <b>HEP_SOFT_DIR_NAME</b>       : {hct(HEP_SOFT_DIR_NAME, '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>SUBMIT_SCRIPT </b>          : {hct(SUBMIT_SCRIPT,     '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>CONFIG_FILE</b>             : {hct(CONFIG_FILE,       '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>README_SIM_FILE_NAME</b>    : {hct(README_SIM_FILE_NAME,   '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>ROOT_CERN_SRC_DIR_NAME </b> : {hct(ROOT_CERN_SRC_DIR_NAME, '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>ROOT_CERN_INS_DIR_NAME</b>  : {hct(ROOT_CERN_INS_DIR_NAME, '#0000FF')}<br/>\n"

        f"    <H4>setup parameters: paths</H4>\n"
        f"    &#x27F6; <b>USER_ROOT_PATH</b>                 : {hct(USER_ROOT_PATH,                  '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>HEP_SOFT_PATH </b>                 : {hct(HEP_SOFT_PATH,                   '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>T2K_ND280_UPGRADE_SOFT_SRC_PATH</b>: {hct(T2K_ND280_UPGRADE_SOFT_SRC_PATH, '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>T2K_ND280_UPGRADE_SOFT_BLD_PATH</b>: {hct(T2K_ND280_UPGRADE_SOFT_BLD_PATH, '#0000FF')}<br/>\n"
        # f"    &#x27F6; <b>ND280UPEVENT_DICTS_PATH</b>        : {hct(ND280UPEVENT_DICTS_PATH,         '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>ND280UPEVENT_DICTS_PATH</b>        : {hct(ND280UPEVENT_DICTS_PATH,         '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>WORK_PATH</b>                      : {hct(WORK_PATH,                       '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>SIM_PATH</b>                       : {hct(SIM_PATH,                        '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>SUBMIT_SCRIPT_PATH</b>             : {hct(SUBMIT_SCRIPT_PATH,              '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>CONFIG_FILE_PATH</b>               : {hct(CONFIG_FILE_PATH,                '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>README_SIM_FILE_PATH</b>           : {hct(README_SIM_FILE_PATH,            '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>ROOT_CERN_SRC_PATH</b>             : {hct(ROOT_CERN_SRC_PATH,              '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>ROOT_CERN_INS_PATH</b>             : {hct(ROOT_CERN_INS_PATH,              '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>GEANT4_PATH</b>                    : {hct(GEANT4_PATH,                     '#0000FF')}<br/>\n"
        f"  </div>\n"

        f"  <div class=simulation-parameters>\n"
        f"    <H4>simulation parameters</H4>\n"
        f"    &#x27F6; Particles: <span style=\"color:green;\">{''.join(f' {particle} ' for particle in particles)}</span><br/>\n"
        "    <span style=\"color:red;\">        Attention: if particle is neutrino,"
        " than momentum vector, kinetic energy and gun position values are not used!!!"
        " In *.mac file generator type is <em>Generator</em> instead of <em>ParticleGun</em>! </span><br/>\n"
        f"    &#x27F6; <b>Magnetic field values</b>: {magnetic_filed_str_info}<br/>\n"
        f"    &#x27F6; <b>Momentum vector values</b>: {momentum_vector_str_info}<br/>\n"
        f"    &#x27F6; <b>Kinetic energy values</b>: {kinetic_energy_str_info}<br/>\n"
        f"    &#x27F6; <b>Gun position values</b>: {gun_position_str_info}<br/>\n"
        f"    &#x27F6; <b>Number of events</b>: {hct(nexpts, '#00FF00')}<br/>\n"
        f"    &#x27F6; <b>Job folder path</b>: {hct(job_folder_path, '#0000FF')}<br/>\n"
        f"    &#x27F6; <b>Build dir path</b>: {hct(build_dir_path, '#0000FF')}<br/>\n"
        f"  </div>\n"

        f"</div>\n"
    )

    # write information about simulation into README file
    with README_SIM_FILE_PATH.open("a") as f:
        f.write(readme_info_string)


def simulate_v2(job_folder_tag="test", build_dir_path=T2K_ND280_UPGRADE_SOFT_BLD_PATH, nexpts=1,
                particles=None, magnetic_field_list=None, momentum_vector_list=None,
                kinetic_energy_list=None, gun_position_list=None,
                main_sim_path=SIM_PATH,
                run_simulation=False):

    if not particles: raise NameError(f"{CERROR} list of particles is not defined! {ENDC}")
    if not magnetic_field_list: raise NameError(f"{CERROR} list of magnetic_field_list is not defined! {ENDC}")
    if not momentum_vector_list and not kinetic_energy_list: raise NameError(f"{CERROR} list of momentum vectors/kinetic energies is not defined! {ENDC}")
    if not gun_position_list: raise NameError(f"{CERROR} list of gun positions is not defined! {ENDC}")

    creation_time = datetime.now()
    job_folder_name = f"{job_folder_tag}-{creation_time.strftime('%d.%m.%Y-%H.%M.%S')}"
    job_folder_path = main_sim_path / job_folder_name

    print(f"{CINFO}{BOLD}\nSimulation configuration:{ENDC}",
          f"\n->{CINFO}particles:{ENDC}\n",
          particles,
          f"\n->{CINFO}magnetic field values:{ENDC}\n",
          magnetic_field_list,
          f"\n->{CINFO}momentum vector values:{ENDC}\n",
          momentum_vector_list,
          f"\n->{CINFO}gun position values:{ENDC}\n",
          gun_position_list,
          f"\n->{CINFO}number of events:{ENDC}\n",
          nexpts,
          f"\n->{CINFO}simulation path:{ENDC}\n",
          f" {CPATH}{job_folder_path}{ENDC}",
          f"\n->{CINFO}software build path:{ENDC}\n",
          f" {CPATH}{build_dir_path}{ENDC}",)

    vis_path, sim_path, anr_path = create_simulation_directories(job_folder_path)

    write_readme_file(particles, magnetic_field_list, momentum_vector_list, kinetic_energy_list, gun_position_list,
                      job_folder_path, build_dir_path, nexpts, creation_time)

    if momentum_vector_list:
        iterator = product(particles, gun_position_list, magnetic_field_list, momentum_vector_list)
        for it in iterator:
            make_vis_file(vis_path=vis_path, particle=it[0],
                          kinetic_energy=None, momentum_vector=it[3],
                          gun_position=it[1], magnetic_field=it[2])
    elif kinetic_energy_list:
        iterator = product(particles, gun_position_list, magnetic_field_list, kinetic_energy_list)
        for it in iterator:
            make_vis_file(vis_path=vis_path, particle=it[0],
                          kinetic_energy=it[3], momentum_vector=None,
                          gun_position=it[1], magnetic_field=it[2])

    # create_parallel_commands_file_v2(job_folder_path, vis_path, sim_path, nexpts)
    parallel_commands_file_path, njobs = create_parallel_commands_file_v3(job_folder_path, vis_path, sim_path, nexpts)

    if run_simulation:
        cmd = (f"nice -n15 "
               "/t2k/users/shvartsman/T2KND280soft_/node204-test-t2k-nd280-upgrade/parallel-20200922-install/bin/parallel"
               f" -j{njobs} -a {parallel_commands_file_path}")
        os.system(cmd)


def prepare_simulation(config):
    msg = (
        "\n"
        f"{CHEADER}*************************************{ENDC}\n"
        f"{CHEADER}********* Simulation starts: ********{ENDC}\n"
        f"{CHEADER}*************************************{ENDC}\n"
        "\n"
        f"{CINFO}Configured global variables:{ENDC}\n"
        f"  {CGLOBALVAR}HEP_SOFT_DIR_NAME               {ENDC}: {HEP_SOFT_DIR_NAME              }\n"
        f"  {CGLOBALVAR}USER_ROOT_DIR_NAME              {ENDC}: {USER_ROOT_DIR_NAME             }\n"
        f"  {CPATH     }USER_ROOT_PATH                  {ENDC}: {USER_ROOT_PATH                 }\n"
        f"  {CPATH     }HEP_SOFT_PATH                   {ENDC}: {HEP_SOFT_PATH                  }\n"
        f"  {CPATH     }T2K_ND280_UPGRADE_SOFT_SRC_PATH {ENDC}: {T2K_ND280_UPGRADE_SOFT_SRC_PATH}\n"
        f"  {CPATH     }T2K_ND280_UPGRADE_SOFT_BLD_PATH {ENDC}: {T2K_ND280_UPGRADE_SOFT_BLD_PATH}\n"
        # f"  {CPATH     }ND280UPEVENT_DICTS_DIR          {ENDC}: {ND280UPEVENT_DICTS_DIR         }\n"
        f"  {CPATH     }ND280UPEVENT_DICTS_PATH         {ENDC}: {ND280UPEVENT_DICTS_PATH        }\n"
        f"  {CPATH     }WORK_PATH                       {ENDC}: {WORK_PATH                      }\n"
        f"  {CPATH     }SIM_PATH                        {ENDC}: {SIM_PATH                       }\n"
        f"  {CPATH     }GENIE_NU_SIM_FILE_PATH          {ENDC}: {GENIE_NU_SIM_FILE_PATH         }\n"
        f"  {CGLOBALVAR}SUBMIT_SCRIPT                   {ENDC}: {SUBMIT_SCRIPT                  }\n"
        f"  {CPATH     }SUBMIT_SCRIPT_PATH              {ENDC}: {SUBMIT_SCRIPT_PATH             }\n"
        f"  {CGLOBALVAR}CONFIG_FILE                     {ENDC}: {CONFIG_FILE                    }\n"
        f"  {CPATH     }CONFIG_FILE_PATH                {ENDC}: {CONFIG_FILE_PATH               }\n"
    )
    print(msg)

    msg_default_config_options = f"\n{CINFO}Detector configuration default options:{ENDC}\n"
    for key, val in SIM_DEFAULT_CONFIG.items():
        msg_default_config_options += f"  {BOLD}{key:30s}{ENDC}: {val}\n"
    print(msg_default_config_options)

    msg_user_config_options = f"\n{CINFO}Detector configuration user options:{ENDC}\n"
    for key, val in config.items():
        msg_user_config_options += f"  {BOLD}{key:30s}{ENDC}: {val}\n"
    print(msg_user_config_options)

    # create SIM_PATH if not exists
    if not SIM_PATH.exists():
        SIM_PATH.mkdir()

    # create soft configuration file if not exists
    submit_file_name = "submit_nd280upgrade_sim-my"
    submit_file_path = SIM_PATH / submit_file_name
    if not submit_file_path.exists():
        create_configuration_file(submit_file_path)

    # create detector configuration file
    config_file_name = "configuration_cluster_SuperFGD-v14.02.2021.xml"
    config_file_path = SIM_PATH / config_file_name
    generate_config_xml(config_file_path, config)

    # create README file if not exists
    if not README_SIM_FILE_PATH.exists():
        with README_SIM_FILE_PATH.open("w") as f:
            f.write("<style>\n"
                    "body {\n"
                    "  display: flex;\n"
                    "  flex-direction: column-reverse;\n"
                    "}\n"
                    ".simulation:hover {\n"
                    "  background-color: #AAAAAA;\n"
                    "}\n"
                    "</style>")
            f.write("<H1>Information about simulations</H1>\n")


def make_vis_file(vis_path,  # str
                  particle,  # str
                  kinetic_energy,  # list 2 or str
                  momentum_vector,  # list 4 or str
                  gun_position,  # list 4
                  magnetic_field,  # list 4 or str
                  genie_nu_file_path=GENIE_NU_SIM_FILE_PATH,
):
    mv = momentum_vector
    ke = kinetic_energy
    mf = magnetic_field
    gp = gun_position

    str_mom = f'Px{mv[0]}y{mv[1]}z{mv[2]}u{mv[3][:-2]}_' if (mv) else ''
    str_ke = f'E{ke[0]}u{ke[1]}_' if (mv is None) else ''
    str_mf = f'Bx{mf[0]}y{mf[1]}z{mf[2]}u{mf[3]}_'
    str_gp = f'GPx{gp[0]}y{gp[1]}z{gp[2]}u{gp[3]}_'

    filename =\
        'ND280_' +\
        f'{particle}_' +\
        str_mom +\
        str_ke +\
        str_gp +\
        str_mf +\
        'vis.mac'

    generator_type = "PG" if particle not in ["nu_mu", "nu_e", "nu_tau"] else "Generator"

    path2file = vis_path / filename

    str2write = (
        # '/tracking/verbose 2 # print many physics informations about each step (daughters,...)\n'
        '/tracking/verbose 1 # print physics informations\n'
        '/process/list\n'
        '/testem/stepMax 1.0 mm\n'
        '/field/setStepperType 4\n'
        '/field/setMinStep 0.1 mm\n'
        f'/field/setField {mf[0]:.9f} {mf[1]} {mf[2]} {mf[3]}\n'
        '/field/update\n'
        '/db/set/saveAllTraj 1\n'
        '/db/set/saveAllHits 1\n')
    if generator_type == "Generator":
        str2write += '/generator/type Generator\n'
    else:
        str2write += (
            '/generator/type ParticleGun\n'
            f'/gun/particle {particle}\n')
        if (momentum_vector):
            str2write += f'/gun/momentum {mv[0]} {mv[1]} {mv[2]} {mv[3][:-2]}\n'
        else:
            str2write += f'/gun/energy {ke[0]} {ke[1]}\n'
        str2write += (
            f'/gun/position {gp[0]} {gp[1]} {gp[2]} {gp[3]}\n'
            '/gun/direction 0 0 1 \n')
    with path2file.open("w") as f:
        f.write(str2write)

    print(f'{CINFO}File "{CFILE}{filename}{CINFO}" at "{CPATH}{vis_path}{CINFO}" is created{ENDC}')


def run_analysis(sim_dirname, root_file, analise_script='AR.C', run_script='run2.py'):
    analisys_time = datetime.now()
    cmd = ('python3 '
           f'{WORK_PATH}/run_scripts/{run_script} '
           f'{WORK_PATH}/an_scr/{analise_script} '
           f'{WORK_PATH}/sim/{sim_dirname}/root-sim/ {root_file} ')
    os.system(cmd)

    readme_info_string = (
        f"<div class=analisys>\n"
        f"  <H3>Analisys at <span style=\"color:red;\">{analisys_time.strftime('%d.%m.%Y %H:%M:%S')}</span></H3>\n"

        f"  <div class=analisys-parameters>\n"
        f"    <H4>analisys parameters</H4>\n"
        f"    &#x27F6; <b>Analisys of *.root file</b>: {root_file}<br/>\n"
        f"    &#x27F6; <b>*.root file path</b>       : {f'{WORK_PATH}/sim/{sim_dirname}/root-sim/{root_file}'}<br/>\n"
        f"    &#x27F6; <b>Analisys script</b>        : {analise_script}<br/>\n"
        f"    &#x27F6; <b>Analisys script path</b>   : {f'{WORK_PATH}/an_scr/{analise_script}'}<br/>\n"
        f"    &#x27F6; <b>Run script</b>             : {run_script}<br/>\n"
        f"    &#x27F6; <b>Run script path</b>        : {f'{WORK_PATH}/run_scripts/{run_script}'}<br/>\n"
        f"  </div>\n"

        f"</div>\n"
    )

    # write information about analisys into README file
    with README_SIM_FILE_PATH.open("a") as f:
        f.write(readme_info_string)


# def run_analysis_v2(sim_dirname, root_file, analise_script='AR.C', main_work_path=WORK_PATH, nd280upevent_dicts_dir=ND280UPEVENT_DICTS_DIR):
def run_analysis_v2(sim_dirname, root_file, analise_script='AR.C', main_work_path=WORK_PATH, nd280upevent_dicts_dir=ND280UPEVENT_DICTS_PATH):
    analisys_time = datetime.now()

    params = f'\\"{main_work_path}/sim/{sim_dirname}/root-sim/\\",\\"{root_file}\\"'
    file_with_parameters = f'"{main_work_path}/an_scr/{analise_script}({params})"'

    cmd = ('root -l -q -b '
           f'{nd280upevent_dicts_dir}/TND280UpTrackPoint.cc+ '
           f'{nd280upevent_dicts_dir}/TND280UpTrack.cc+ '
           f'{nd280upevent_dicts_dir}/TND280UpVertex.cc+ '
           f'{nd280upevent_dicts_dir}/TND280UpHit.cc+ '
           f'{nd280upevent_dicts_dir}/TND280UpEvent.cc+ '
           f"{file_with_parameters}")
    os.system(cmd)

    readme_info_string = (
        f"<div class=analisys>\n"
        f"  <H3>Analisys at <span style=\"color:red;\">{analisys_time.strftime('%d.%m.%Y %H:%M:%S')}</span></H3>\n"

        f"  <div class=analisys-parameters>\n"
        f"    <H4>analisys parameters</H4>\n"
        f"    &#x27F6; <b>Analisys of *.root file</b>: {root_file}<br/>\n"
        f"    &#x27F6; <b>*.root file path</b>       : {f'{WORK_PATH}/sim/{sim_dirname}/root-sim/{root_file}'}<br/>\n"
        f"    &#x27F6; <b>Analisys script</b>        : {analise_script}<br/>\n"
        f"    &#x27F6; <b>Analisys script path</b>   : {f'{WORK_PATH}/an_scr/{analise_script}'}<br/>\n"
        f"  </div>\n"

        f"</div>\n"
    )

    # write information about analisys into README file
    with README_SIM_FILE_PATH.open("a") as f:
        f.write(readme_info_string)


def create_documentation(source_dir=f"{T2K_ND280_UPGRADE_SOFT_SRC_PATH / 'src'}"):
    raise NotImplementedError
    for root_folder, subdirs, files in os.walk(source_dir):
        for filename in files:
            if filename.endswith(".cc") or filename.endswith(".hh") or filename.endswith(".h") or filename.endswith(".c"):
                with open(os.path.join(root_folder, filename)) as f:
                    file_content = f.readlines()
                    for line in file_content:
                        # 1. detect imports: they define general project sructure (#import)
                        # 2. detect global variables (#define)
                        # 3. detect classes (class)
                        # 4. detect functions
                        # 5. detect local variables in function body
                        if line.startswith("#import"):
                            pass
                        elif line.startswith("#define"):
                            pass
                        elif line.startswith("class"):
                            pass
                    pass


def build(build_dir_tag, nice=None):
    print(os.getcwd())
    os.chdir(ND280UP_SOFT_DIR)
    build_dir = f'build-{build_dir_tag}'
    try:
        os.system(f"mkdir -p -v {build_dir}")
    except OSError as e:
        print(f"Creation of the directory {build_dir} failed", e)
    except:
        print(f"Creation of the directory {build_dir} failed")
    else:
        print(f"Successfully created the directory {build_dir}")
    os.chdir(os.path.join(ND280UP_SOFT_DIR, build_dir))
    # os.system('rm -rf *')
    os.system(f'rm {ND280UP_SOFT_DIR}/src/app/nd280UpEvent/*.pcm '
              f'{ND280UP_SOFT_DIR}/src/app/nd280UpEvent/*.so '
              f'{ND280UP_SOFT_DIR}/src/app/nd280UpEvent/*.d')
    os.system(f'cmake {ND280UP_SOFT_DIR}/src '
              '-DGEANT4_CONFIG_EXECUTABLE=$GEANT4_DIR/bin/geant4-config '
              '-DGEANT4_LIBRARY_DIR=$GEANT4_DIR/lib64/ '
              '-DGEANT4_INCLUDE_DIR=$GEANT4_DIR/include/Geant4/')
    nice_str = f'nice -n{nice} ' if nice else ''
    os.system(f'{nice_str}make -j8')
    os.chdir(WORK_PATH)


def build_v2(build_dir_tag, nice=None):
    nice_str = f'nice -n{nice} ' if nice else ''
    build_dir = f'build-{build_dir_tag}'
    build_script = f"""
        #!/bin/bash

        . /opt/rh/devtoolset-3/enable
        . /t2k/users/suvorov/stable/ROOT/root-6.12.06-build/bin/thisroot.sh
        . /t2k/users/suvorov/stable/GEANT4/geant4.10.01.p03-install/bin/geant4.sh
        export GEANT4_DIR=/t2k/users/suvorov/stable/GEANT4/geant4.10.01.p03-install/
        export GEANT4_INCLUDE=$GEANT4_DIR/include/Geant4/
        export GEANT4_INSTALL_BIN=$GEANT4_DIR/bin/
        export GEANT4_LIBRARY=$GEANT4_DIR/lib64/
        export GEANT4_LIBRARY_DIR=$GEANT4_DIR/lib64/
        export GEANT4_CONFIG_EXECUTABLES=$GEANT4_DIR/bin/geant4-config

        echo "removing {ND280UPEVENT_DICTS_DIR}/*.pcm"
        rm {ND280UPEVENT_DICTS_DIR}/*.pcm
        echo "removing {ND280UPEVENT_DICTS_DIR}/*.so"
        rm {ND280UPEVENT_DICTS_DIR}/*.so
        echo "removing {ND280UPEVENT_DICTS_DIR}/*.d"
        rm {ND280UPEVENT_DICTS_DIR}/*.d

        cd {ND280UP_SOFT_DIR}
        mkdir -p -v {build_dir}
        cd {build_dir}

        cmake ../src \
          -DGEANT4_CONFIG_EXECUTABLE=$GEANT4_DIR/bin/geant4-config \
          -DGEANT4_LIBRARY_DIR=$GEANT4_DIR/lib64/ \
          -DGEANT4_INCLUDE_DIR=$GEANT4_DIR/include/Geant4/
        {nice_str}make -j8
        cd {WORK_PATH}

        # cd t2k-nd280-upgrade/build; rm -rf *;
        # rm ../src/app/nd280UpEvent/*.pcm ../src/app/nd280UpEvent/*.so ../src/app/nd280UpEvent/*.d
        # cmake ../src \
        # -DGEANT4_CONFIG_EXECUTABLE=$GEANT4_DIR/bin/geant4-config \
        # -DGEANT4_LIBRARY_DIR=$GEANT4_DIR/lib64/ \
        # -DGEANT4_INCLUDE_DIR=$GEANT4_DIR/include/Geant4/ \
        # && make -j8
        # cd ../../
    """
    os.system(build_script)


def parse_cmd_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('-a', '--action',
                        type=str,
                        help='enter type of action')
    parser.add_argument('-t', '--tag',
                        type=str,
                        default=T2K_ND280_UPGRADE_SOFT_BLD_PATH,
                        help='tag of build diretory')
    parser.add_argument('-sd', '--sim_dir_name',
                        type=str,
                        help='name of directory containing simulation files')
    parser.add_argument('-sf', '--sim_file_name',
                        type=str,
                        help='name of simulation file')
    parser.add_argument('-sfl', '--sim_file_names_list',
                        type=str,
                        nargs='+',
                        help='names of simulation files')
    parser.add_argument('-anf', '--with_analisys_file',
                        default='AR.C',
                        type=str,
                        help='name of analisys file')
    parser.add_argument('-runf', '--with_run_file',
                        default='run2.py',
                        type=str,
                        help='name of run file')
    parser.add_argument('-j', '--jobs',
                        default=1,
                        type=int,
                        help='number of parallel processes')
    parser.add_argument('-p', '--particles',
                        default='e-',
                        type=str,
                        help='particles')
    parser.add_argument('-mf', '--magnetic-fields',
                        default='0.2,0,0,tesla;',
                        type=str,
                        help='magnetic field list')
    parser.add_argument('-mom', '--momentums',
                        default='',
                        type=str,
                        help='momentum list')
    parser.add_argument('-ek', '--initial-kinetic-energy',
                        default='400,MeV;',
                        type=str,
                        help='initial kinetic energy list')
    parser.add_argument('-gp', '--gun-positions',
                        default='0.5,0.5,-92.0,cm;',
                        type=str,
                        help='gun positions list')
    parser.add_argument('-c', '--simulation-config',
                        default='',
                        type=str,
                        help='simulation config file path')
    parser.add_argument('-df', '--default-config',
                        default='',
                        type=str,
                        help='default config file path')
    # parser.add_argument('-dc', '--default-config',
    #                     help='print default config')
    # parser.add_argument('-mfl', '--with_magnetic_field',
    #                     type=list,
    #                     help='magnetic field')
    parser.add_argument('-n', '--number_of_events',
                        type=int,
                        help='number of events')
    parser.add_argument('-s', '--short',
                        help='print info in short', action='store_true')

    args = parser.parse_args()
    print(args)
    return args


def main_v2(args, particles, magnetic_field_list, momentum_vector_list, kinetic_energy_list, gun_position_list, config):
    if args.action == 'build':
        # os.system('source ./setup-nd280up-soft.sh')
        build_v2(args.tag, None)
    elif args.action == 'prepare':
        prepare_simulation(config)
        simulate_v2(args.sim_dir_name, args.tag, args.number_of_events,
                    particles, magnetic_field_list, momentum_vector_list,
                    kinetic_energy_list, gun_position_list,
                    run_simulation=False)
        print(f"{CHEADER}*************************************{ENDC}\n"
              f"{CHEADER}******** Simulation prepared! *******{ENDC}\n"
              f"{CHEADER}*************************************{ENDC}\n")
    elif args.action == 'simulate':
        prepare_simulation(config)
        simulate_v2(args.sim_dir_name, args.tag, args.number_of_events,
                    particles, magnetic_field_list, momentum_vector_list,
                    kinetic_energy_list, gun_position_list,
                    run_simulation=True)
        print(f"{CHEADER}*************************************{ENDC}\n"
              f"{CHEADER}********** Simulation done! *********{ENDC}\n"
              f"{CHEADER}*************************************{ENDC}\n")
    elif args.action == 'analize':
        if args.sim_file_name:
            # run_analysis(
            #     args.sim_dir_name,
            #     args.sim_file_name,
            #     args.with_analisys_file,
            #     args.with_run_file)
            run_analysis_v2(
                args.sim_dir_name,
                args.sim_file_name,
                args.with_analisys_file)
        elif args.sim_file_names_list:
            lst_of_processes = [
                mp.Process(
                    target=run_analysis,
                    args=(
                        args.sim_dir_name,
                        sim_file,
                        args.with_analisys_file,
                        args.with_run_file
                    )
                )
                for sim_file in args.sim_file_names_list
            ]

            list(map(lambda p: p.start(), lst_of_processes))
            list(map(lambda p: p.join(), lst_of_processes))
    elif args.action == 'document':
        pass
    elif args.short:
        print_short_help()


def store_settings_to_json(settings):
    filepath = 'default_config_settings.json'
    with open(filepath, 'w') as f:
        json.dump(settings, f)
    return f"{os.getcwd()}/{filepath}"


def load_settings_from_json(filepath):
    with open(filepath) as f:
        settings = json.load(f)
    return settings


def print_short_help():
    cpyexecutor = f'{CPYEXECUTOR}{BOLD}python3{ENDC}'
    cscriptpath = f'{CFILE}{CURRENT_SCRIPT_PATH}/main.py{ENDC}'
    print(
        f"{BOLD}Prepare simulation{ENDC}: {cpyexecutor} {cscriptpath} -a prepare -anf CR1.C -sd <> -n <>\n"
        f"{BOLD}Run simulation{ENDC}    : {cpyexecutor} {cscriptpath} -a simulate -anf CR1.C -sd <> -n <>\n"
        f"{BOLD}Run analisys  {ENDC}    : {cpyexecutor} {cscriptpath} -a analize -sd <> -sf <> \n"
    )


if __name__ == '__main__':
    particles = [
        # "mu-",
        "pi-",
        "pi+",
        # "proton",
        # "mu+",
        # "e-",
        # "nu_mu",
        # "nu_e"
    ]

    magnetic_field_list = [
        # (0.000000000001, 0, 0, 'tesla'),
        # (0.000000001, 0, 0, 'tesla'),
        # (0.000001, 0, 0, 'tesla'),
        (0.2, 0, 0, 'tesla'),  # default
        # (20, 0, 0, 'tesla'),
    ]

    momentum_vector_list = [
        # (0, 0, 700, 'MeV/c'),
        # (0, 0, 800, 'MeV/c'),
        # (0, 0, 1000, 'MeV/c'),
        # (0, 0, 400, 'MeV/c'),
    ]

    kinetic_energy_list = [
        # (3, 'MeV'),
        # (5, 'MeV'),
        # (7, 'MeV'),
        # (10, 'MeV'),
        # (20, 'MeV'),
        # (40, 'MeV'),
        # (50, 'MeV'),
        # (60, 'MeV'),
        # (80, 'MeV'),
        # (100, 'MeV'),
        # (200, 'MeV'),
        # (300, 'MeV'),
        (400, 'MeV'),
        # (500, 'MeV'),
        # (600, 'MeV'),
        # (700, 'MeV'),
        # (750, 'MeV'),
        # (800, 'MeV'),
        # (900, 'MeV'),
        # (1000, 'MeV'),
        # (1100, 'MeV'),
    ]

    gun_position_list = [
        (0.5, 0.5, -92.0, 'cm'),  # center of cube !
        # (0.995, 0.5, -92.0, 'cm'),  # reflector !
        (0.2375, 0.3, -92.0, 'cm'),  # hole Fz (center) !
        # (0.3, 0.3, -92.0, 'cm'),  # Fz (center) !
        # (0.7, 0.3, -92.0, 'cm'), # Fy (center) !
        # (0.7625, 0.3, -92.0, 'cm'), # holeFy (center)
        # (0.3, 0.7, -92.0, 'cm'), # Fx (center)
        # (0.3, 0.6375, -92.0, 'cm'), # holeFx (center)
        # (0.7, 0.7, -92.0, 'cm') # FxFy (center) !
        # (0.7, 0.6375, -92.0, 'cm'), # Fx holeFy (center)
        # (0.7625, 0.6375, -92.0, 'cm'), # holeFx, holeFy (center)
    ]

    # config = {
    #     "Path2file": "/t2k/users/shvartsman/T2KND280soft_/",
    #     "filename": "GENIE_geometry_SuperFGD_1e21_numu.root",
    # }

    config = {
        "Path2file": "/t2k/users/shvartsman/T2KND280soft_/node204-new-scheme/",
        "filename": "GENIE_G18_10b_1e22_nue_FHC.root",
    }

    args = parse_cmd_args()
    # main_v2(args, particles, magnetic_field_list, momentum_vector_list, kinetic_energy_list, gun_position_list, config)
    
    ''' store settings
    file = store_settings_to_json(SIM_DEFAULT_CONFIG)
    print(file)
    print(load_settings_from_json(file))
    #  '''

    # example:
    # python3 main.py -a simulate -t no_mu_decays -sd mu-_Bx0_no-decays -n 1000
    # python3 /t2k/users/shvartsman/T2KND280soft_/node207-01-t2k-nd280-upgrade/main.py -a analize -anf BR.C -sd <> -sf <>
    # python3 /t2k/users/shvartsman/T2KND280soft_/node204-new-scheme/main.py -a analize -anf CR.C -sd <> -sf <>
    # python3 main.py -a simulate -sd mu-_Bx0_no-decays -n 1000
