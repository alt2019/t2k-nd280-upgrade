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
import sys
import subprocess
import multiprocessing as mp

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
    self.mp_parallel_args:list = []
    self.dir_with_sim_root_files:Path = None

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

    if "N_EVENTS_IN_PARALLEL" in GLOBAL_PARAMETERS:
      self.N_events_in_parallel = int(GLOBAL_PARAMETERS["N_EVENTS_IN_PARALLEL"])
    else:
      self.N_events_in_parallel = 100

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
        GENIE_CONFIG = config_parser["GENIE_CONFIG"]
        particles = GENIE_CONFIG["particles"]
        if " " in particles: particles_list = particles.split(" ")
        elif "," in particles: particles_list = particles.split(",")
        else: particles_list = [particles]
        self.particles_list = particles_list
        print(particles_list)

        magnetic_field = GENIE_CONFIG["magnetic_field"]
        if "," in magnetic_field: magnetic_field_list = magnetic_field.split(",")
        else: magnetic_field_list = [magnetic_field]
        self.magnetic_field_list = magnetic_field_list
        print(magnetic_field_list)

        if "kinetic_energy" in GENIE_CONFIG:
          kinetic_energy = GENIE_CONFIG["kinetic_energy"]
          if "," in kinetic_energy: kinetic_energy_list = kinetic_energy.split(",")
          else: kinetic_energy_list = [kinetic_energy]
          self.kinetic_energy_list = kinetic_energy_list
          print(kinetic_energy_list)

        momentum_list = []
        if "momentum" in GENIE_CONFIG:
          momentum = GENIE_CONFIG["momentum"]
          if "," in momentum: momentum_list = momentum.split(",")
          else: momentum_list = [momentum]
          self.momentum_list = momentum_list
          print(momentum_list)

        self.N_events_to_simulate = int(GENIE_CONFIG["N_events_to_simulate"])
        print(self.N_events_to_simulate)
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
          # '/db/set/saveAllHits 0\n'
          # 'TCoating 0.2 mm\n'
        )
      if generator_type == "Generator":
          str2write += '/generator/type Generator\n'

          # if (energy_measure_type == "mom"):
          #   # str2write += f'/gun/momentum {mv[0]} {mv[1]} {mv[2]} {mv[3][:-2]}\n'
          #   str2write += f"/gun/momentumAmp {energy_measure_item.replace('/c', '')}\n"
          # elif (energy_measure_type == "ene"):
          #   str2write += f'/gun/energy {energy_measure_item}\n'
          # else:
          #   pass
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
      # raise Exception("Momentum or kinetic energy are not set!")
      iterator = product(
        self.particles_list,
        self.magnetic_field_list,
      )
      for it in iterator:
        print(it)
        self._create_geant4_vis_file(
          vis_path = vis_path,
          particle = it[0],
          energy_measure_item = None,
          energy_measure_type = None,
          gun_position = None,
          gun_direction = None,
          magnetic_field = it[1],
        )
      return

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

    N_iterations = 1
    N_expts_remained = 0
    if self.N_events_in_parallel < nexpts:
      N_iterations = nexpts // self.N_events_in_parallel
      N_expts_remained = nexpts - N_iterations*self.N_events_in_parallel

    njobs = 0
    data2write = []
    for vis_file in vis_files_lst:
        # print(filename)
        filename = vis_file.name

        '''
        ### indicator for simulation process
        # /t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/test-12.02.2024-19.28.51/root-sim/ND280_mu-_P700uMeVc_GPx91.5y26.5z-92.0ucm_GDx0y0z1_Bx0.2y0z0utesla_n1000-Exp0-Nexpt1000
        out_fname_tag = f"{filename[:-8]}-Exp0-Nexpt{nexpts}" ## filename w/o '-vis.mac' suffix
        log_file_path = f"{sim_path}/log-{out_fname_tag}.log"
        command_itself = f"{self.t2knd280up_soft_build_path}/app/EffStudy/EffStudyApp {vis_file} {cfg_path} {sim_path}/p-{out_fname_tag} 0 {nexpts} &>> {log_file_path}"

        ## !!!NEW!!!
        # out_fname_tag = f"{filename[:-8]}-Exp{start_idx}-Nexpt{n_events_to_process}" ## filename w/o '-vis.mac' suffix
        # log_file_path = f"{sim_path}/log-{out_fname_tag}.log"
        # command_itself = f"{self.t2knd280up_soft_build_path}/app/EffStudy/EffStudyApp {vis_file} {cfg_path} {sim_path}/p-{out_fname_tag} {start_idx} {n_events_to_process} &>> {log_file_path}"

        cmd_rename_root_if_ok = f"mv {sim_path}/p-{out_fname_tag}.root {sim_path}/{out_fname_tag}.root"
        cmd_rename_root_if_err = f"mv {sim_path}/p-{out_fname_tag}.root {sim_path}/sv$i-{out_fname_tag}.root"
        cmd_rename_log_if_err = f"mv {log_file_path} {sim_path}/sv$i-log-{out_fname_tag}.log"

        # post_action_condition = f"grep -q 'segmentation violation' '{log_file_path}'"
        post_action_condition = f"tail -n 100 {log_file_path} | grep -q 'segmentation violation'" ## for large files just grep is very slow
        # post_action_on_err = f"{cmd_rename_root_if_err} && {cmd_rename_log_if_err} && echo '{out_fname_tag} $i' >> {sim_path}/err.txt"
        post_action_on_err = f"{cmd_rename_root_if_err} && {cmd_rename_log_if_err} && echo {out_fname_tag} $i >> {sim_path}/err.txt"
        post_action_on_ok = f"{cmd_rename_root_if_ok}; break"
        post_action = f"if {post_action_condition}; then {post_action_on_err}; else {post_action_on_ok}; fi"
        # _str2write = f"for i in {{0..5}}; do {command_itself}; {post_action}; done"
        _str2write = f"for i in {{0..100}}; do {command_itself}; {post_action}; done"

        data2write.append(_str2write)
        njobs += 1
        #'''

        for i in range(N_iterations+1): ## !!!NEW!!!
          start_idx = i * self.N_events_in_parallel          
          if i == N_iterations:
            if N_expts_remained != 0:
              n_events_to_process = N_expts_remained
            else:
              break
          else:
            n_events_to_process = self.N_events_in_parallel

          ### indicator for simulation process
          # /t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/test-12.02.2024-19.28.51/root-sim/ND280_mu-_P700uMeVc_GPx91.5y26.5z-92.0ucm_GDx0y0z1_Bx0.2y0z0utesla_n1000-Exp0-Nexpt1000
          ## !!!NEW!!!
          out_fname_tag = f"{filename[:-8]}-Exp{start_idx}-Nexpt{n_events_to_process}" ## filename w/o '-vis.mac' suffix
          log_file_path = f"{sim_path}/log-{out_fname_tag}.log"
          command_itself = f"{self.t2knd280up_soft_build_path}/app/EffStudy/EffStudyApp {vis_file} {cfg_path} {sim_path}/p-{out_fname_tag} {start_idx} {n_events_to_process} &>> {log_file_path}"

          ''' ### worked
          cmd_rename_root_if_ok = f"mv {sim_path}/p-{out_fname_tag}.root {sim_path}/{out_fname_tag}.root"
          cmd_rename_root_if_err = f"mv {sim_path}/p-{out_fname_tag}.root {sim_path}/sv$i-{out_fname_tag}.root"
          cmd_rename_log_if_err = f"mv {log_file_path} {sim_path}/sv$i-log-{out_fname_tag}.log"

          # post_action_condition = f"grep -q 'segmentation violation' '{log_file_path}'"
          post_action_condition = f"tail -n 100 {log_file_path} | grep -q 'segmentation violation'" ## for large files just grep is very slow
          # post_action_on_err = f"{cmd_rename_root_if_err} && {cmd_rename_log_if_err} && echo '{out_fname_tag} $i' >> {sim_path}/err.txt"
          post_action_on_err = f"{cmd_rename_root_if_err} && {cmd_rename_log_if_err} && echo {out_fname_tag} $i >> {sim_path}/err.txt"
          post_action_on_ok = f"{cmd_rename_root_if_ok}; break"
          post_action = f"if {post_action_condition}; then {post_action_on_err}; else {post_action_on_ok}; fi"
          # _str2write = f"for i in {{0..5}}; do {command_itself}; {post_action}; done"
          _str2write = f"for i in {{0..100}}; do {command_itself}; {post_action}; done"

          data2write.append(_str2write)
          njobs += 1
          #'''

          """
          EffStudyApp {vis_file} {cfg_path} {sim_path}/p-{out_fname_tag} {start_idx} {n_events_to_process} &>> {log_file_path}
          ## tail -n {log_file_path} | grep "'ND280RootPersistencyManager::Store' finished"
          tail -n {log_file_path} | grep "Finished storing event "
          [DBG]/t2k/users/shvartsman/soft/split/n211/scl-sfgd-02/t2k-nd280-upgrade-fork/src/app/EffStudy/src/ND280RootPersistencyManager.cc:1532:Store:Finished storing event 
          """

          app_exe_path = f"{self.t2knd280up_soft_build_path}/app/EffStudy/EffStudyApp"
          vis_file_path = vis_file
          xml_config_path = cfg_path
          sim_out_path = sim_path
          outfile_tag = f"{filename[:-8]}"
          start_event_num = start_idx
          stop_event_num = start_event_num + n_events_to_process
          base_level = 0
          self.mp_parallel_args.append([self, str(app_exe_path), str(vis_file_path), str(xml_config_path), str(sim_out_path),
                                        outfile_tag, start_event_num, stop_event_num, n_events_to_process, base_level])
          # self.mp_parallel_args.append([str(app_exe_path), str(vis_file_path), str(xml_config_path), str(sim_out_path),
          #                               outfile_tag, start_event_num, stop_event_num, n_events_to_process, base_level])

          run_sim_script_path = "/t2k/users/shvartsman/soft/split/n211/scl-sfgd-02/t2k-nd280-upgrade-fork/src/app/py-manage"
          cmd = (
            f"python {run_sim_script_path}/run_simulation.py"
            f" {app_exe_path} {vis_file_path} {xml_config_path} {sim_out_path}"
            f" {outfile_tag} {start_event_num} {stop_event_num} {n_events_to_process} {base_level}"
          )

          data2write.append(cmd)

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

    self.dir_with_sim_root_files = sim_path

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

  def run_simulation_with_error_checking_v2(self):
    self.prepare_simulation()

    cmd = f"nice -n15 parallel -j{self.N_parallel_threads} -a {self.parallel_commands_file_path}"
    os.system(cmd)

    return

    command = [
      "nice",
      "-n15",
      "parallel",
      "-j",
      str(self.N_parallel_threads),
      "-a",
      str(self.parallel_commands_file_path)
    ]

    try:
      output = subprocess.check_output(command)
      print(output.decode("utf-8"))  # Decode the output if it's in bytes

      created_root_files_d = dict()
      for file in self.dir_with_sim_root_files.iterdir():
        fname = file.name
        if fname.startswith("ND280_") and fname.endswith(".root"):
          idx_of_Exp = fname.find("Exp")
          idx_of_ = fname[idx_of_Exp:].find("_")
          print(fname, idx_of_Exp, idx_of_)
          print(fname[idx_of_Exp+3:idx_of_Exp+idx_of_])
          num = int(fname[idx_of_Exp+3:idx_of_Exp+idx_of_])
          tag = fname[len("ND280_"):idx_of_Exp-1]
          if tag in created_root_files_d:
            created_root_files_d[tag].append((str(file), num))
          else:
            created_root_files_d[tag] = [(str(file), num)]
      for tag, files_list in created_root_files_d.items():
        files_list = sorted(files_list, key=lambda item: item[1])
        files_list_s = "\n".join([fl[0] for fl in files_list])
        with open(f"{self.output_path}/root_files-{tag}.list", "w") as f: f.write(files_list_s)
    except subprocess.CalledProcessError as e:
      print("Command execution failed:", e)

  def run_simulation_with_error_checking(self):
    self.prepare_simulation()

    # pool = mp.Pool(processes=20)

    #'''
    proc_list = []
    # print(self.mp_parallel_args)
    for it in self.mp_parallel_args:
      print(it)
      p = mp.Process(target = MySFGDManager._simulate_with_error_checking, args = it)
      proc_list.append(p)
      # p.start()

    N_processes = len(proc_list)
    print("N processes: ", N_processes)

    max_threads_to_use = 32
    # if N_processes > max_threads_to_use:
    #   iterations = N_processes // max_threads_to_use
    proc_list_final = [proc_list[i * max_threads_to_use:(i + 1) * max_threads_to_use] for i in range((len(proc_list) + max_threads_to_use - 1) // max_threads_to_use )]
    for pps in proc_list_final:
      # print(pps)
      for p in pps: p.start()
      for p in pps: p.join()

    # for p in proc_list: p.join()
    #'''

    # max_threads_to_use = 32
    # pool = mp.Pool(processes=max_threads_to_use)
    # # results = [pool.apply(MySFGDManager._simulate_with_error_checking, args=(args,)) for args in self.mp_parallel_args]
    # results = [pool.apply(MySFGDManager._simulate_with_error_checking2, args=args) for args in self.mp_parallel_args]
    # # pool.map(MySFGDManager._simulate_with_error_checking2, self.mp_parallel_args)

    # # pool.map(MySFGDManager._simulate_with_error_checking, self.mp_parallel_args)

    # created_root_files = []
    # tags_s = set()
    created_root_files_d = dict()
    for file in self.dir_with_sim_root_files.iterdir():
      fname = file.name
      if fname.startswith("ND280_") and fname.endswith(".root"):
        idx_of_Exp = fname.find("Exp")
        idx_of_ = fname[idx_of_Exp:].find("_")
        print(fname, idx_of_Exp, idx_of_)
        print(fname[idx_of_Exp+3:idx_of_Exp+idx_of_])
        num = int(fname[idx_of_Exp+3:idx_of_Exp+idx_of_])
        tag = fname[len("ND280_"):idx_of_Exp-1]
        if tag in created_root_files_d:
          created_root_files_d[tag].append((str(file), num))
        else:
          created_root_files_d[tag] = [(str(file), num)]

        # tags_s.insert(tag)
        # created_root_files.append((str(file), tag))

    # created_root_files_s = "\n".join(created_root_files)

    # with open(f"{self.output_path}/root-files.list", "w") as f:
    #   f.write(created_root_files_s)

    for tag, files_list in created_root_files_d.items():
      files_list = sorted(files_list, key=lambda item: item[1])
      files_list_s = "\n".join([fl[0] for fl in files_list])
      with open(f"{self.output_path}/root_files-{tag}.list", "w") as f: f.write(files_list_s)


  def _simulate_with_error_checking(self, app_exe_path:str, vis_file_path:str, xml_config_path:str,
                                          sim_out_path:str, outfile_tag:str,
                                          start_event_num:int, stop_event_num:int, n_events_to_process:int,
                                          level:int):
    n_events_to_process = stop_event_num - start_event_num
    of = f"{outfile_tag}-Exp{start_event_num}-Nexpt{n_events_to_process}-L{level}"
    outfile_tag_wo_ext = f"{sim_out_path}/p-{of}" ## p -- indicator that file in progress
    outfile_fullpath = f"{outfile_tag_wo_ext}.root"
    success_command_indicator = f"Output file {outfile_fullpath} closed."

    print(app_exe_path, vis_file_path, xml_config_path,
          sim_out_path, outfile_tag,
          start_event_num, stop_event_num, n_events_to_process,
          level)

    # print([
    #     app_exe_path, vis_file_path, xml_config_path, outfile_tag_wo_ext, f'{start_event_num}', f'{n_events_to_process}'
    #   ])

    try:
      output = subprocess.check_output([
        app_exe_path, vis_file_path, xml_config_path, outfile_tag_wo_ext, f'{start_event_num}', f'{n_events_to_process}'
      ], stderr=subprocess.PIPE)
      decoded_output = output.decode(sys.getfilesystemencoding())
      decoded_output_list = decoded_output.split("\n")
      decoded_output_last10_strings = "\n".join(decoded_output_list[-10:])
      print(decoded_output_last10_strings)
      #Output file /t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/root-sim/p-ND280_nu_e_Bx0.2y0z0utesla-Exp600-Nexpt100-00.root closed.
      idx = decoded_output_last10_strings.find(success_command_indicator)
      if idx != -1:
        of_new = f"{outfile_tag}-Exp{start_event_num}_{stop_event_num-1}"
        outfile_fullpath_new = f"{sim_out_path}/{of_new}.root"
        os.rename(outfile_fullpath, outfile_fullpath_new)
        log_file_path = f"{sim_out_path}/log-{of_new}.log"
        # log_file_path = f"{sim_out_path}/log-{of}.log"
        with open(log_file_path, "w") as f: f.write(decoded_output)
        return
    except subprocess.CalledProcessError as e:
      print('exit code: {}'.format(e.returncode))
      #print('stdout: {}'.format(e.output.decode(sys.getfilesystemencoding())))
      decoded_stdout = e.output.decode(sys.getfilesystemencoding())
      decoded_stdout_list = decoded_stdout.split("\n")
      decoded_stdout_last50_strings = "\n".join(decoded_stdout_list[-50:])
      print(decoded_stdout_last50_strings)
      decoded_stderr = e.stderr.decode(sys.getfilesystemencoding())
      print('stderr: {}'.format(decoded_stderr))
            
      n = len("Finished storing event")
      idx = decoded_stdout_last50_strings.find("Finished storing event")
      if idx >= 0:
        idx_newline = decoded_stdout_last50_strings.find("\n", idx)
        point_of_interest = decoded_stdout_last50_strings[idx+n+1:idx_newline]
        event_id_with_broken_hits_deletion = int(point_of_interest)
        print(f"event_id_with_broken_hits_deletion: {event_id_with_broken_hits_deletion} [{start_event_num}<->{stop_event_num}]")

        of_new = f"{outfile_tag}-Exp{start_event_num}_{event_id_with_broken_hits_deletion}_b" ## las 'b' means broken at this event
        outfile_fullpath_new = f"{sim_out_path}/{of_new}.root"
        os.rename(outfile_fullpath, outfile_fullpath_new)

        ## check for safe way to rite in file with multiprocessing
        # with open(f"{sim_out_path}/progress.errlog", "w") as f: f.write(f"Break at event {event_id_with_broken_hits_deletion}")

        with open(f"{outfile_tag_wo_ext}.errlog", "w") as f:
          f.write(f"Break at event {event_id_with_broken_hits_deletion} [{start_event_num}<->{stop_event_num}]")
        # log_file_path_stdout = f"{sim_out_path}/log-{of}.stdout.log"
        log_file_path_stdout = f"{sim_out_path}/log-{of_new}.stdout.log"
        with open(log_file_path_stdout, "w") as f: f.write(decoded_stdout)
        # log_file_path_stderr = f"{sim_out_path}/log-{of}.stderr.log"
        log_file_path_stderr = f"{sim_out_path}/log-{of_new}.stderr.log"
        with open(log_file_path_stderr, "w") as f: f.write(decoded_stderr)

        if (self.sim_generator_type == "ParticleGun"):
          start_event_num_new = start_event_num + event_id_with_broken_hits_deletion + 1
        elif (self.sim_generator_type == "Genie"):
          start_event_num_new = event_id_with_broken_hits_deletion + 1
        n_events_to_process_new = stop_event_num - start_event_num_new
        # n_events_to_process_new = n_events_to_process - start_event_num
        level_new = level + 1

        self._simulate_with_error_checking(app_exe_path, vis_file_path, xml_config_path, sim_out_path, outfile_tag,
                                          start_event_num_new, stop_event_num, n_events_to_process_new, level_new)

  @staticmethod
  def _simulate_with_error_checking2(app_exe_path:str, vis_file_path:str, xml_config_path:str,
                                          sim_out_path:str, outfile_tag:str,
                                          start_event_num:int, stop_event_num:int, n_events_to_process:int,
                                          level:int):
    n_events_to_process = stop_event_num - start_event_num
    of = f"{outfile_tag}-Exp{start_event_num}-Nexpt{n_events_to_process}-L{level}"
    outfile_tag_wo_ext = f"{sim_out_path}/p-{of}" ## p -- indicator that file in progress
    outfile_fullpath = f"{outfile_tag_wo_ext}.root"
    success_command_indicator = f"Output file {outfile_fullpath} closed."

    print(app_exe_path, vis_file_path, xml_config_path,
          sim_out_path, outfile_tag,
          start_event_num, stop_event_num, n_events_to_process,
          level)

    # print([
    #     app_exe_path, vis_file_path, xml_config_path, outfile_tag_wo_ext, f'{start_event_num}', f'{n_events_to_process}'
    #   ])

    try:
      output = subprocess.check_output([
        app_exe_path, vis_file_path, xml_config_path, outfile_tag_wo_ext, f'{start_event_num}', f'{n_events_to_process}'
      ], stderr=subprocess.PIPE)
      decoded_output = output.decode(sys.getfilesystemencoding())
      decoded_output_list = decoded_output.split("\n")
      decoded_output_last10_strings = "\n".join(decoded_output_list[-10:])
      print(decoded_output_last10_strings)
      #Output file /t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/root-sim/p-ND280_nu_e_Bx0.2y0z0utesla-Exp600-Nexpt100-00.root closed.
      idx = decoded_output_last10_strings.find(success_command_indicator)
      if idx != -1:
        of_new = f"{outfile_tag}-Exp{start_event_num}_{stop_event_num-1}"
        outfile_fullpath_new = f"{sim_out_path}/{of_new}.root"
        os.rename(outfile_fullpath, outfile_fullpath_new)
        log_file_path = f"{sim_out_path}/log-{of_new}.log"
        # log_file_path = f"{sim_out_path}/log-{of}.log"
        with open(log_file_path, "w") as f: f.write(decoded_output)
        return
    except subprocess.CalledProcessError as e:
      print('exit code: {}'.format(e.returncode))
      #print('stdout: {}'.format(e.output.decode(sys.getfilesystemencoding())))
      decoded_stdout = e.output.decode(sys.getfilesystemencoding())
      decoded_stdout_list = decoded_stdout.split("\n")
      decoded_stdout_last50_strings = "\n".join(decoded_stdout_list[-50:])
      print(decoded_stdout_last50_strings)
      decoded_stderr = e.stderr.decode(sys.getfilesystemencoding())
      print('stderr: {}'.format(decoded_stderr))
            
      n = len("Finished storing event")
      idx = decoded_stdout_last50_strings.find("Finished storing event")
      if idx >= 0:
        idx_newline = decoded_stdout_last50_strings.find("\n", idx)
        point_of_interest = decoded_stdout_last50_strings[idx+n+1:idx_newline]
        event_id_with_broken_hits_deletion = int(point_of_interest)
        print(f"event_id_with_broken_hits_deletion: {event_id_with_broken_hits_deletion} [{start_event_num}<->{stop_event_num}]")

        of_new = f"{outfile_tag}-Exp{start_event_num}_{event_id_with_broken_hits_deletion}_b" ## las 'b' means broken at this event
        outfile_fullpath_new = f"{sim_out_path}/{of_new}.root"
        os.rename(outfile_fullpath, outfile_fullpath_new)

        ## check for safe way to rite in file with multiprocessing
        # with open(f"{sim_out_path}/progress.errlog", "w") as f: f.write(f"Break at event {event_id_with_broken_hits_deletion}")

        with open(f"{outfile_tag_wo_ext}.errlog", "w") as f:
          f.write(f"Break at event {event_id_with_broken_hits_deletion} [{start_event_num}<->{stop_event_num}]")
        # log_file_path_stdout = f"{sim_out_path}/log-{of}.stdout.log"
        log_file_path_stdout = f"{sim_out_path}/log-{of_new}.stdout.log"
        with open(log_file_path_stdout, "w") as f: f.write(decoded_stdout)
        # log_file_path_stderr = f"{sim_out_path}/log-{of}.stderr.log"
        log_file_path_stderr = f"{sim_out_path}/log-{of_new}.stderr.log"
        with open(log_file_path_stderr, "w") as f: f.write(decoded_stderr)

        start_event_num_new = event_id_with_broken_hits_deletion + 1
        n_events_to_process_new = stop_event_num - start_event_num_new
        # n_events_to_process_new = n_events_to_process - start_event_num
        level_new = level + 1

        MySFGDManager._simulate_with_error_checking2(app_exe_path, vis_file_path, xml_config_path, sim_out_path, outfile_tag,
                                          start_event_num_new, stop_event_num, n_events_to_process_new, level_new)

    


  def __test(self):
    pass
    #```
    '''
    import sys
    import subprocess

    def simulate(app_exe_path:str, vis_file_path:str, xml_config:str, outfile_dirtag:str, start_event_num:int, nexpts:int):
      # app_exe_path = '/t2k/users/shvartsman/soft/split/n211/scl-sfgd-02/t2k-nd280-upgrade-fork-build-09/app/EffStudy/EffStudyApp'
      # vis_file_path = '/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/vis-files/ND280_nu_e_Bx0.2y0z0utesla-vis.mac'
      # xml_config = '/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/config-files/configuration_cluster_SuperFGD-v14.02.2021.xml'
      # outfile_dirtag = '/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/root-sim/p-ND280_nu_e_Bx0.2y0z0utesla-Exp600-Nexpt100-00'
      # start_event_num = 600
      # nexpts = 100
      outfile_fullpath = f"{outfile_dirtag}.root"
      success_command_indicator = f"Output file {outfile_fullpath} closed."
      while True:
        try:
            output = subprocess.check_output([app_exe_path, vis_file_path, xml_config, outfile_dirtag, f'{start_event_num}', f'{nexpts}'], stderr=subprocess.PIPE)
            decoded_output = output.decode(sys.getfilesystemencoding())
            decoded_output_list = decoded_output.split("\n")
            decoded_output_last10_strings = "\n".join(decoded_output_list[-10:])
            print(decoded_output_last10_strings)
            #Output file /t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/root-sim/p-ND280_nu_e_Bx0.2y0z0utesla-Exp600-Nexpt100-00.root closed.
            idx = decoded_output_last10_strings.find(success_command_indicator)
            if idx != -1: break
        except subprocess.CalledProcessError as e:
            print('exit code: {}'.format(e.returncode))
            #print('stdout: {}'.format(e.output.decode(sys.getfilesystemencoding())))
            decoded_stdout = e.output.decode(sys.getfilesystemencoding())
            decoded_stdout_list = decoded_stdout.split("\n")
            decoded_stdout_last50_strings = "\n".join(decoded_stdout_list[-50:])
            print(decoded_stdout_last50_strings)
            print('stderr: {}'.format(e.stderr.decode(sys.getfilesystemencoding())))
            
            n = len("Finished storing event")
            idx = decoded_stdout_last50_strings.find("Finished storing event")
            if idx < 0: pass
            
            idx_newline = decoded_stdout_last50_strings.find("\n", idx)
            point_of_interest = decoded_stdout_last50_strings[idx+n+1:idx_newline]
            # print("point_of_interest", int(point_of_interest))
            event_id_with_broken_hits_deletion = int(point_of_interest)
            print("event_id_with_broken_hits_deletion", event_id_with_broken_hits_deletion)

    def simulate2(app_exe_path:str, vis_file_path:str, xml_config:str, outfile_dirtag:str, start_event_num:int, nexpts:int):
      # app_exe_path = '/t2k/users/shvartsman/soft/split/n211/scl-sfgd-02/t2k-nd280-upgrade-fork-build-09/app/EffStudy/EffStudyApp'
      # vis_file_path = '/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/vis-files/ND280_nu_e_Bx0.2y0z0utesla-vis.mac'
      # xml_config = '/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/config-files/configuration_cluster_SuperFGD-v14.02.2021.xml'
      # outfile_dirtag = '/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/root-sim/p-ND280_nu_e_Bx0.2y0z0utesla-Exp600-Nexpt100-00'
      # start_event_num = 600
      # nexpts = 100
      -> # outfile_fullpath = f"{outfile_dirtag}.root"
      -> outfile_fullpath = f"{outfile_dirtag}-Exp{start_event_num}-Nexpt{nexpts}.root"
      success_command_indicator = f"Output file {outfile_fullpath} closed."

      -> TODO: split 'outfile_dirtag' into 'file_tag' and its 'parent_dir'


      try:
          output = subprocess.check_output([
            app_exe_path, vis_file_path, xml_config, outfile_dirtag, f'{start_event_num}', f'{nexpts}'
          ], stderr=subprocess.PIPE)
          decoded_output = output.decode(sys.getfilesystemencoding())
          decoded_output_list = decoded_output.split("\n")
          decoded_output_last10_strings = "\n".join(decoded_output_list[-10:])
          print(decoded_output_last10_strings)
          #Output file /t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/nue-genie-0.5GeV-CCQE-rnd_vtx-2000evts-02-2024.03.29-23.48.34/root-sim/p-ND280_nu_e_Bx0.2y0z0utesla-Exp600-Nexpt100-00.root closed.
          idx = decoded_output_last10_strings.find(success_command_indicator)
          if idx != -1: return
      except subprocess.CalledProcessError as e:
          print('exit code: {}'.format(e.returncode))
          #print('stdout: {}'.format(e.output.decode(sys.getfilesystemencoding())))
          decoded_stdout = e.output.decode(sys.getfilesystemencoding())
          decoded_stdout_list = decoded_stdout.split("\n")
          decoded_stdout_last50_strings = "\n".join(decoded_stdout_list[-50:])
          print(decoded_stdout_last50_strings)
          print('stderr: {}'.format(e.stderr.decode(sys.getfilesystemencoding())))
            
          n = len("Finished storing event")
          idx = decoded_stdout_last50_strings.find("Finished storing event")
          if idx < 0: pass

          idx_newline = decoded_stdout_last50_strings.find("\n", idx)
          point_of_interest = decoded_stdout_last50_strings[idx+n+1:idx_newline]
          event_id_with_broken_hits_deletion = int(point_of_interest)
          print("event_id_with_broken_hits_deletion", event_id_with_broken_hits_deletion)

          with open(f"{outfile_dirtag}.errlog", "w") as f: f"Break at event {event_id_with_broken_hits_deletion}"

          start_event_num_new = event_id_with_broken_hits_deletion + 1
          nexpts_new = nexpts - start_event_num_new
          -> outfile_fullpath_new = f"{outfile_dirtag}-Exp{start_event_num_new}-Nexpt{nexpts_new}.root" 

          simulate2(app_exe_path, vis_file_path, xml_config, outfile_dirtag, start_event_num_new, nexpts_new)
    # ```
    #'''