#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import argparse
from MySFGDManager import MySFGDManager


def parse_cmd_args():
  parser = argparse.ArgumentParser()

  parser.add_argument('-c', '--config',
                      type=str,
                      help='path for config file')
  parser.add_argument('-j', '--jobs',
                      type=str,
                      help='number of parallel jobs')

  args = parser.parse_args()
  return args

if __name__ == '__main__':
  args = parse_cmd_args()
  m = MySFGDManager(config=args.config)
  # m.prepare_simulation()
  # m.run_simulation()
  m.run_simulation_with_error_checking()
  # m.run_simulation_with_error_checking_v2()

  ## example:
  # python3 run.py --config configs/e_mu_config.ini
