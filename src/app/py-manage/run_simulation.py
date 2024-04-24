import os
import sys
import subprocess


def _simulate_with_error_checking(app_exe_path:str, vis_file_path:str, xml_config_path:str,
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
      # start_event_num_new = start_event_num + event_id_with_broken_hits_deletion + 1
      n_events_to_process_new = stop_event_num - start_event_num_new
      # n_events_to_process_new = n_events_to_process - start_event_num
      level_new = level + 1

      _simulate_with_error_checking(app_exe_path, vis_file_path, xml_config_path, sim_out_path, outfile_tag,
                                    start_event_num_new, stop_event_num, n_events_to_process_new, level_new)


def _simulate_with_error_checking2(app_exe_path:str, vis_file_path:str, xml_config_path:str,
                                  sim_out_path:str, outfile_tag:str,
                                  start_event_num:int, stop_event_num:int, n_events_to_process:int,
                                  level:int):

  n_events_to_process = stop_event_num - start_event_num
  of = f"{outfile_tag}-Exp{start_event_num}-Nexpt{n_events_to_process}-L{level}"
  outfile_tag_wo_ext = f"{sim_out_path}/p-{of}" ## p -- indicator that file in progress
  outfile_fullpath = f"{outfile_tag_wo_ext}.root"
  success_command_indicator = f"Output file {outfile_fullpath} closed."

  while (True):
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
        break

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

        start_event_num = start_event_num_new
        n_events_to_process = n_events_to_process_new
        level = level_new


if __name__ == "__main__":
  app_exe_path = sys.argv[1]
  vis_file_path = sys.argv[2]
  xml_config_path = sys.argv[3]
  sim_out_path = sys.argv[4]
  outfile_tag = sys.argv[5]
  start_event_num = int(sys.argv[6])
  stop_event_num = int(sys.argv[7])
  n_events_to_process = int(sys.argv[8])
  level = int(sys.argv[9])

  # _simulate_with_error_checking(
  #   app_exe_path=app_exe_path, vis_file_path=vis_file_path, xml_config_path=xml_config_path, sim_out_path=sim_out_path,
  #   outfile_tag=outfile_tag, start_event_num=start_event_num, stop_event_num=stop_event_num,
  #   n_events_to_process=n_events_to_process, level=level)
  _simulate_with_error_checking2(
    app_exe_path=app_exe_path, vis_file_path=vis_file_path, xml_config_path=xml_config_path, sim_out_path=sim_out_path,
    outfile_tag=outfile_tag, start_event_num=start_event_num, stop_event_num=stop_event_num,
    n_events_to_process=n_events_to_process, level=level)