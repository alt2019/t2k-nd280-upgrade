#!/usr/bin/env python
# -*- coding: UTF-8 -*-

from configparser import ExtendedInterpolation, ConfigParser
from rich import print as rprint
from pathlib import Path
from datetime import datetime
from itertools import product
import xml.etree.ElementTree as ET
from xml.dom.minidom import parseString
from collections import OrderedDict
import shutil
import os

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


class MySFGDManager:
  def __init__(self, config:str):
    self.config_file_path = Path(config)
    
    self.base_soft_path:Path = None
    self.output_simrecoana_path:Path = None
    self.action:str = None

    self.N_parallel_threads:int = None
    self.tag:str = None
    self.description:str = None

    self.t2knd280up_soft_src_path:Path = None
    self.t2knd280up_soft_build_path:Path = None

    self.sfgd_config:OrderedDict = OrderedDict()

    ## action = simulate
    self.sim_generator_type:str = []
    self.particles_list:list = []
    self.kinetic_energy_list:list = []
    self.momentum_list:list = []
    self.magnetic_field_list:list = []
    self.gun_position_list:list = []
    self.gun_direction_list:list = []
    self.N_events_to_simulate:int = None
    self.geant4_verbosity_level:int = None

    ## output
    self.output_path:Path = None
    self.parallel_commands_file_path:Path = None

    self.read_config()

  def read_config(self):
    config_parser = ConfigParser(interpolation=ExtendedInterpolation())
    config_parser.optionxform = lambda option: option ## for reading ini w/o conversion to lowercase
    config_parser.read(self.config_file_path)
    print(config_parser)

    ## reading section with global config paraemters
    GLOBAL_PARAMETERS = config_parser["GLOBAL_PARAMETERS"]
    self.base_soft_path = Path(GLOBAL_PARAMETERS["BASE_SOFT_PATH"])
    self.output_simrecoana_path = Path(GLOBAL_PARAMETERS["OUTPUT_SIMRECOANA_PATH"])
    self.N_parallel_threads = GLOBAL_PARAMETERS["PARALLEL_THREADS"]
    self.action = GLOBAL_PARAMETERS["ACTION"]
    self.tag = GLOBAL_PARAMETERS["TAG"]
    self.description = GLOBAL_PARAMETERS["DESCRIPTION"]

    SFGD_SIMULATION_DEFAULT_CONFIG = config_parser["SFGD_SIMULATION_DEFAULT_CONFIG"]
    for k in SFGD_SIMULATION_DEFAULT_CONFIG:
      print(k)
      print(k, SFGD_SIMULATION_DEFAULT_CONFIG[k])
      self.sfgd_config[k] = SFGD_SIMULATION_DEFAULT_CONFIG[k]

    SFGD_SIMULATION_CONFIG = config_parser["SFGD_SIMULATION_CONFIG"]
    for k in SFGD_SIMULATION_CONFIG:
      self.sfgd_config[k] = SFGD_SIMULATION_CONFIG[k]


    T2K_ND280_UPGRADE_SOFT = config_parser["T2K_ND280_UPGRADE_SOFT"]
    self.t2knd280up_soft_src_path = Path(T2K_ND280_UPGRADE_SOFT["T2K_ND280_UPGRADE_SOFT_SRC_PATH"])
    self.t2knd280up_soft_build_path = Path(T2K_ND280_UPGRADE_SOFT["T2K_ND280_UPGRADE_SOFT_BLD_PATH"])
    print(self.t2knd280up_soft_src_path)
    print(self.t2knd280up_soft_build_path)
    print(self.base_soft_path)
    print(self.output_simrecoana_path)
    print(self.N_parallel_threads)
    print(self.action)
    if (self.action == "simulate"):
      EffStudy_config = config_parser["EffStudy_config"]
      self.geant4_verbosity_level = EffStudy_config["GEANT4_VERBOSITY_LEVEL"]
      self.sim_generator_type = EffStudy_config["GENERATOR"]
      if (self.sim_generator_type == "ParticleGun"):
        PARTICLEGUN_CONFIG = config_parser["PARTICLEGUN_CONFIG"]
        particles = PARTICLEGUN_CONFIG["particles"]
        if " " in particles: particles_list = particles.split(" ")
        elif "," in particles: particles_list = particles.split(",")
        else: particles_list = [particles]
        self.particles_list = particles_list
        print(particles_list)
        
        # kinetic_energy_list = []
        if "kinetic_energy" in PARTICLEGUN_CONFIG:
          kinetic_energy = PARTICLEGUN_CONFIG["kinetic_energy"]
          if "," in kinetic_energy: kinetic_energy_list = kinetic_energy.split(",")
          else: kinetic_energy_list = [kinetic_energy]
          self.kinetic_energy_list = kinetic_energy_list
          print(kinetic_energy_list)

        momentum_list = []
        if "momentum" in PARTICLEGUN_CONFIG:
          momentum = PARTICLEGUN_CONFIG["momentum"]
          if "," in momentum: momentum_list = momentum.split(",")
          else: momentum_list = [momentum]
          self.momentum_list = momentum_list
          print(momentum_list)

        magnetic_field = PARTICLEGUN_CONFIG["magnetic_field"]
        if "," in magnetic_field: magnetic_field_list = magnetic_field.split(",")
        else: magnetic_field_list = [magnetic_field]
        self.magnetic_field_list = magnetic_field_list
        print(magnetic_field_list)

        gun_position = PARTICLEGUN_CONFIG["gun_position"]
        if "," in gun_position: gun_position_list = gun_position.split(",")
        else: gun_position_list = [gun_position]
        self.gun_position_list = gun_position_list
        print(gun_position_list)

        gun_direction_list = []
        gun_direction = PARTICLEGUN_CONFIG["gun_direction"]
        if gun_direction:
          if "," in gun_direction: gun_direction_list = gun_direction.split(",")
          else: gun_direction_list = [gun_direction]
          print(gun_direction_list)
        if not gun_direction_list: gun_direction_list = ["0 0 1"]
        self.gun_direction_list = gun_direction_list

        self.N_events_to_simulate = int(PARTICLEGUN_CONFIG["N_events_to_simulate"])
        print(self.N_events_to_simulate)
        
      elif (self.sim_generator_type == "Genie"):
        pass
    elif (self.action == "reconstruct"):
      pass
    elif (self.action == "visualize"):
      pass

  def create_simulation_directories(self):
    vis_path = self.output_path / "vis-files"
    cfg_path = self.output_path / "config-files"
    sim_path = self.output_path / 'root-sim'
    anr_path = self.output_path / 'an-res'

    self.output_path.mkdir(parents=True, exist_ok=True)
    vis_path.mkdir(parents=True, exist_ok=True)
    cfg_path.mkdir(parents=True, exist_ok=True)
    sim_path.mkdir(parents=True, exist_ok=True)
    anr_path.mkdir(parents=True, exist_ok=True)

    print(f"\n{CINFO}Directories:\n"
          f"    {CPATH}{self.output_path}{ENDC}\n"
          f"    {CPATH}{vis_path}{ENDC}\n"
          f"    {CPATH}{sim_path}{ENDC}\n"
          f"    {CPATH}{anr_path}{ENDC}\n"
          f"  {CINFO}are created{ENDC}\n")

    return vis_path, cfg_path, sim_path, anr_path

  def _create_geant4_vis_file(self,
    vis_path:Path,
    particle:str = "e-",
    energy_measure_item:str = "500 Mev", ## or "500 MeV/c"
    energy_measure_type: str = "ene", ## or "mom"
    gun_position:str = "0.5 0.5 -92.0 cm",
    gun_direction:str = "0 0 1",
    magnetic_field:str = "0.2 0 0 tesla",
  ):    
      mfl = magnetic_field.split()
      str_mf = f"Bx{mfl[0]}y{mfl[1]}z{mfl[2]}u{mfl[3]}"

      ### create filename (different for Genie and ParticleGun)
      if particle not in ["nu_mu", "nu_e", "nu_tau"]:
        energy_measure_lbl = {
          "ene": "E",
          "mom": "P",
        }
        em = energy_measure_item.split()
        str_eml = f"{energy_measure_lbl[energy_measure_type]}{em[0]}u{em[1].replace('/', '')}"
        gpl = gun_position.split()
        str_gp = f"GPx{gpl[0]}y{gpl[1]}z{gpl[2]}u{gpl[3]}"
        gdl = gun_direction.split()
        str_gd = f"GDx{gdl[0]}y{gdl[1]}z{gdl[2]}"

        filename = f"ND280_{particle}_{str_eml}_{str_gp}_{str_gd}_{str_mf}-vis.mac"
        generator_type = "PG"
      else:
        filename = f"ND280_{particle}_{str_mf}-vis.mac"
        generator_type = "Generator"
      
      str2write = (
          # '/tracking/verbose 2 # print many physics informations about each step (daughters,...)\n'
          f'/tracking/verbose {self.geant4_verbosity_level} # print physics informations\n'
          '/process/list\n'
          '/testem/stepMax 1.0 mm\n'
          '/field/setStepperType 4\n'
          '/field/setMinStep 0.1 mm\n'
          f'/field/setField {magnetic_field}\n'
          '/field/update\n'
          '/db/set/saveAllTraj 1\n'
          '/db/set/saveAllHits 1\n'
          # 'TCoating 0.2 mm\n'
        )
      if generator_type == "Generator":
          str2write += '/generator/type Generator\n'
      else:
          str2write += (
              '/generator/type ParticleGun\n'
              f'/gun/particle {particle}\n')
          if (energy_measure_type == "mom"):
              # str2write += f'/gun/momentum {mv[0]} {mv[1]} {mv[2]} {mv[3][:-2]}\n'
              str2write += f"/gun/momentumAmp {energy_measure_item.replace('/c', '')}\n"
          else:
              str2write += f'/gun/energy {energy_measure_item}\n'
          str2write += (
              f'/gun/position {gun_position}\n'
              f'/gun/direction {gun_direction} \n' if gun_direction else '/gun/direction 0 0 1 \n'
          )
      print(str2write)
      path2file = vis_path / filename
      with path2file.open("w") as f:
        f.write(str2write)

      print(f'{CINFO}File "{CFILE}{filename}{CINFO}" at "{CPATH}{vis_path}{CINFO}" is created{ENDC}')


  def create_geant4_vis_files(self, vis_path:Path):
    # print("!!!!")
    if self.momentum_list:
      energy_measure_list = self.momentum_list
      energy_measure_type = "mom"
    elif self.kinetic_energy_list:
      energy_measure_list = self.kinetic_energy_list
      energy_measure_type = "ene"
    else:
      raise Exception("Momentum or kinetic energy are not set!")

    print(energy_measure_list)
    print(self.particles_list)
    print(self.gun_position_list)
    print(self.gun_direction_list)
    print(self.magnetic_field_list)

    iterator = product(
      self.particles_list,
      self.gun_position_list,
      self.gun_direction_list,
      self.magnetic_field_list,
      energy_measure_list
    )
    for it in iterator:
      print(it)
      self._create_geant4_vis_file(
        vis_path = vis_path,
        particle = it[0],
        energy_measure_item = it[4],
        energy_measure_type = energy_measure_type,
        gun_position = it[1],
        gun_direction = it[2],
        magnetic_field = it[3],
      )

  def generate_config_xml(self, file_path:Path):
    file_structure = ET.Element('fileStructure')
    main_packg = ET.SubElement(file_structure, 'Main_Package')
    main_packg.text = "File_Navigate"
    dep_det = ET.SubElement(file_structure, 'Dependency_Details')
    dep = ET.SubElement(dep_det, 'Dependency')

    for key, val in self.sfgd_config.items():
        print(key)
        item = ET.SubElement(dep, key)
        text = "\n" + val + "\n      "
        item.text = text

    fs = ET.tostring(file_structure, encoding="utf-8")

    dom = parseString(fs)
    dom_str = dom.toprettyxml(indent="  ")
    rprint(dom_str)
    with file_path.open("w") as f:
      f.write(dom_str)

    msg = (f"{CINFO}Created detector configuration xml file "
           f"\"{CFILE}{file_path.name}{ENDC}\""
           f"{CINFO} at "
           f"\"{CPATH}{file_path.parent}{ENDC}\"")
    print(msg)

  def create_parallel_commands_file(self, vis_path:Path, sim_path:Path, cfg_path:Path, nexpts:int):
    parallel_commands_file_path = self.output_path / "parallel-commands.txt"
    vis_files_lst = vis_path.glob("*.mac")

    njobs = 0
    data2write = []
    for vis_file in vis_files_lst:
        # print(filename)
        filename = vis_file.name

        ### indicator for simulation process
        # /t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/test-12.02.2024-19.28.51/root-sim/ND280_mu-_P700uMeVc_GPx91.5y26.5z-92.0ucm_GDx0y0z1_Bx0.2y0z0utesla_n1000-Exp0-Nexpt1000
        out_fname_tag = f"{filename[:-8]}-Exp0-Nexpt{nexpts}" ## filename w/o '-vis.mac' suffix
        log_file_path = f"{sim_path}/log-{out_fname_tag}.log"
        command_itself = f"{self.t2knd280up_soft_build_path}/app/EffStudy/EffStudyApp {vis_file} {cfg_path} {sim_path}/p-{out_fname_tag} 0 {nexpts} &>> {log_file_path}"

        cmd_rename_root_if_ok = f"mv {sim_path}/p-{out_fname_tag}.root {sim_path}/{out_fname_tag}.root"
        cmd_rename_root_if_err = f"mv {sim_path}/p-{out_fname_tag}.root {sim_path}/sv$i-{out_fname_tag}.root"
        cmd_rename_log_if_err = f"mv {log_file_path} {sim_path}/sv$i-log-{out_fname_tag}.log"

        # post_action_condition = f"grep -q 'segmentation violation' '{log_file_path}'"
        post_action_condition = f"tail -n 100 {log_file_path} | grep -q 'segmentation violation'" ## for large files just grep is very slow
        # post_action_on_err = f"{cmd_rename_root_if_err} && {cmd_rename_log_if_err} && echo '{out_fname_tag} $i' >> {sim_path}/err.txt"
        post_action_on_err = f"{cmd_rename_root_if_err} && {cmd_rename_log_if_err} && echo {out_fname_tag} $i >> {sim_path}/err.txt"
        post_action_on_ok = f"{cmd_rename_root_if_ok}; break"
        post_action = f"if {post_action_condition}; then {post_action_on_err}; else {post_action_on_ok}; fi"
        _str2write = f"for i in {{0..5}}; do {command_itself}; {post_action}; done"

        data2write.append(_str2write)
        njobs += 1

    str2write = "\n".join(data2write)
    with parallel_commands_file_path.open('w') as f:
      f.write(str2write)

    print(f'{CINFO}File "{CFILE}{parallel_commands_file_path.name}'
          f'{CINFO}" at "{CPATH}{parallel_commands_file_path.parent}'
          f'{CINFO}" is created{ENDC}')
    return parallel_commands_file_path, njobs

  def prepare_simulation(self):
    creation_time = datetime.now()
    # job_folder_name = f"{self.tag}-{creation_time.strftime('%d.%m.%Y-%H.%M.%S')}"
    job_folder_name = f"{self.tag}-{creation_time.strftime('%Y.%m.%d-%H.%M.%S')}"
    self.output_path = self.output_simrecoana_path / f"{self.action}/{job_folder_name}"

    vis_path, cfg_path, sim_path, anr_path = self.create_simulation_directories()
    self.create_geant4_vis_files(vis_path=vis_path)

    config_file_name = "configuration_cluster_SuperFGD-v14.02.2021.xml"
    config_file_path = cfg_path / config_file_name
    self.generate_config_xml(file_path=config_file_path)
    
    shutil.copy(self.config_file_path, cfg_path)

    self.parallel_commands_file_path, njobs = self.create_parallel_commands_file(
      vis_path=vis_path, sim_path=sim_path,
      cfg_path=config_file_path,
      nexpts=self.N_events_to_simulate
    )

  def run_simulation(self):
    self.prepare_simulation()

    cmd = f"nice -n15 parallel -j{self.N_parallel_threads} -a {self.parallel_commands_file_path}"
    os.system(cmd)