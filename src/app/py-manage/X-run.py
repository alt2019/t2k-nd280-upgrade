#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import os
import argparse
from datetime import datetime
import pathlib
from pathlib import Path
import multiprocessing as mp
import multiprocessing
import xml.etree.ElementTree as ET
from xml.dom.minidom import parseString
from itertools import product
import json
# import configparser
from configparser import ExtendedInterpolation, ConfigParser
from rich import print as rprint

from MySFGDManager import MySFGDManager


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
####################################################################################################



def parse_cmd_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('-c', '--config',
                        type=str,
                        help='path for config file')

    args = parser.parse_args()
    # print(args, args.config)
    return args

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

def make_vis_file(vis_path:Path,
                  particle:str = "e-",
                  kinetic_energy:str = "500 Mev",
                  momentum:str = "500 MeV/c",
                  gun_position:str = "0.5 0.5 -92.0 cm",
                  gun_direction:str = "0 0 1",
                  magnetic_field:str = "0.2 0 0 tesla",
):
    mv = momentum
    ### check for momentum
    if momentum:
        mvl = momentum.split()
        str_kin = f"P{mvl[0]}u{mvl[1].replace('/', '')}"
    elif kinetic_energy:
        kel = kinetic_energy.split()
        str_kin = f"E{kel[0]}u{kel[1]}"
    mfl = magnetic_field.split()
    str_mf = f"Bx{mfl[0]}y{mfl[1]}z{mfl[2]}u{mfl[3]}"
    gpl = gun_position.split()
    str_gp = f"GPx{gpl[0]}y{gpl[1]}z{gpl[2]}u{gpl[3]}"
    gdl = gun_direction.split()
    str_gd = f"GDx{gdl[0]}y{gdl[1]}z{gdl[2]}"

    filename = f"ND280_{particle}_{str_kin}_{str_gp}_{str_gd}_{str_mf}-vis.mac"

    generator_type = "PG" if particle not in ["nu_mu", "nu_e", "nu_tau"] else "Generator"
    
    str2write = (
        # '/tracking/verbose 2 # print many physics informations about each step (daughters,...)\n'
        '/tracking/verbose 1 # print physics informations\n'
        '/process/list\n'
        '/testem/stepMax 1.0 mm\n'
        '/field/setStepperType 4\n'
        '/field/setMinStep 0.1 mm\n'
        f'/field/setField {magnetic_field}\n'
        '/field/update\n'
        '/db/set/saveAllTraj 1\n'
        '/db/set/saveAllHits 1\n')
    if generator_type == "Generator":
        str2write += '/generator/type Generator\n'
    else:
        str2write += (
            '/generator/type ParticleGun\n'
            f'/gun/particle {particle}\n')
        if (momentum):
            # str2write += f'/gun/momentum {mv[0]} {mv[1]} {mv[2]} {mv[3][:-2]}\n'
            str2write += f"/gun/momentumAmp {momentum.replace('/c', '')}\n"
        else:
            str2write += f'/gun/energy {kinetic_energy}\n'
        str2write += (
            f'/gun/position {gun_position}\n'
            f'/gun/direction {gun_direction} \n' if gun_direction else '/gun/direction 0 0 1 \n'
        )
    print(str2write)
    path2file = vis_path / filename
    # with path2file.open("w") as f:
    #     f.write(str2write)

    print(f'{CINFO}File "{CFILE}{filename}{CINFO}" at "{CPATH}{vis_path}{CINFO}" is created{ENDC}')


def run_simulation_particlegun(simpath:Path, lparticles:list, lmom:list, lekin:list, lgunpos:list, lgundir:list, lmagneticf:list):
#   print(len(lparticles), lparticles)
#   print(len(lmom), lmom)
#   print(len(lekin), lekin)

  if lmom:
    iterator = product(lparticles, lgunpos, lgundir, lmagneticf, lmom)
    for it in iterator:
      make_vis_file(
        vis_path = Path(""),
        particle = it[0],
        kinetic_energy = None,
        momentum = it[4],
        gun_position = it[1],
        gun_direction = it[2],
        magnetic_field = it[3],
      )
  elif lekin:
    iterator = product(lparticles, lgunpos, lgundir, lmagneticf, lekin)
    for it in iterator:
      make_vis_file(
        vis_path = Path(""),
        particle = it[0],
        kinetic_energy = it[4],
        momentum = None,
        gun_position = it[1],
        gun_direction = it[2],
        magnetic_field = it[3],
      )



def main(cmd_args):
    config_file_path = Path(cmd_args.config)
    print(config_file_path)

    config_parser = ConfigParser(interpolation=ExtendedInterpolation())
    config_parser.read(config_file_path)
    print(config_parser)

    ## reading section with global config paraemters
    GLOBAL_PARAMETERS = config_parser["GLOBAL_PARAMETERS"]
    BASE_SOFT_PATH = GLOBAL_PARAMETERS["BASE_SOFT_PATH"]
    OUTPUT_SIMRECOANA_PATH = GLOBAL_PARAMETERS["OUTPUT_SIMRECOANA_PATH"]
    PARALLEL_THREADS = GLOBAL_PARAMETERS["PARALLEL_THREADS"]
    ACTION = GLOBAL_PARAMETERS["ACTION"]

    T2K_ND280_UPGRADE_SOFT = config_parser["T2K_ND280_UPGRADE_SOFT"]
    # T2K_ND280_UPGRADE_SOFT_SRC_DIRNAME = T2K_ND280_UPGRADE_SOFT["T2K_ND280_UPGRADE_SOFT_SRC_DIRNAME"]
    # T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME = T2K_ND280_UPGRADE_SOFT["T2K_ND280_UPGRADE_SOFT_BLD_DIRNAME"]
    T2K_ND280_UPGRADE_SOFT_SRC_PATH = Path(T2K_ND280_UPGRADE_SOFT["T2K_ND280_UPGRADE_SOFT_SRC_PATH"])
    T2K_ND280_UPGRADE_SOFT_BLD_PATH = Path(T2K_ND280_UPGRADE_SOFT["T2K_ND280_UPGRADE_SOFT_BLD_PATH"])

    print(T2K_ND280_UPGRADE_SOFT_SRC_PATH)
    print(T2K_ND280_UPGRADE_SOFT_BLD_PATH)

    print(BASE_SOFT_PATH)
    print(OUTPUT_SIMRECOANA_PATH)
    print(PARALLEL_THREADS)
    print(ACTION)

    if (ACTION == "simulate"):
        EffStudy_config = config_parser["EffStudy_config"]
        GENERATOR = EffStudy_config["GENERATOR"]

        if (GENERATOR == "ParticleGun"):
            PARTICLEGUN_CONFIG = config_parser["PARTICLEGUN_CONFIG"]
            particles = PARTICLEGUN_CONFIG["particles"]
            if " " in particles: particles_list = particles.split(" ")
            elif "," in particles: particles_list = particles.split(",")
            else: particles_list = [particles]
            print(particles_list)
            
            kinetic_energy = PARTICLEGUN_CONFIG["kinetic_energy"]
            if "," in kinetic_energy: kinetic_energy_list = kinetic_energy.split(",")
            else: kinetic_energy_list = [kinetic_energy]
            print(kinetic_energy_list)

            momentum_list = []
            if "momentum" in PARTICLEGUN_CONFIG:
                momentum = PARTICLEGUN_CONFIG["momentum"]
                if "," in momentum: momentum_list = momentum.split(",")
                else: momentum_list = [momentum]
                print(momentum_list)

            magnetic_field = PARTICLEGUN_CONFIG["magnetic_field"]
            if "," in kinetic_energy: magnetic_field_list = magnetic_field.split(",")
            else: magnetic_field_list = [magnetic_field]
            print(magnetic_field_list)

            gun_position = PARTICLEGUN_CONFIG["gun_position"]
            if "," in gun_position: gun_position_list = gun_position.split(",")
            else: gun_position_list = [gun_position]
            print(gun_position_list)

            gun_direction_list = []
            gun_direction = PARTICLEGUN_CONFIG["gun_direction"]
            if gun_direction:
                if "," in gun_direction: gun_direction_list = gun_direction.split(",")
                else: gun_direction_list = [gun_direction]
                print(gun_direction_list)
            if not gun_direction_list: gun_direction_list = ["0 0 1"]

            N_events_to_simulate = int(PARTICLEGUN_CONFIG["N_events_to_simulate"])
            print(N_events_to_simulate)

            run_simulation_particlegun(
                simpath = Path(),
                lparticles = particles_list,
                lmom = momentum_list,
                lekin = kinetic_energy_list,
                lgunpos = gun_position_list,
                lgundir = gun_direction_list,
                lmagneticf = magnetic_field_list
            )
            
        elif (GENERATOR == "Genie"):
            pass

    elif (ACTION == "reconstruct"):
        pass
    elif (ACTION == "visualize"):
        pass

    
    

    print(EffStudy_config)

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
    # main(cmd_args=args)
    m = MySFGDManager(config=args.config)
    m.prepare_simulation()
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
