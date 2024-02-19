
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable


# files = [
#   "/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/e_mu-2024.02.14-23.47.50/an-res/ND280_e-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
#   "/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/e_mu-2024.02.14-23.47.50/an-res/ND280_mu-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
#   "/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/ae_amu-2024.02.16-00.13.12/an-res/ND280_e+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
#   "/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate/ae_amu-2024.02.16-00.13.12/an-res/ND280_mu+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
# ]

simdir = "/t2k2/users/shvartsman/PhD-work/SFGD/t2k_nd280_upgrade/simulate"
e_mu_dir = "e_mu-2024.02.19-08.35.04"
ae_amu_dir = "ae_amu-2024.02.19-08.34.16"

files = [
  f"{simdir}/{e_mu_dir}/an-res/ND280_e-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
  f"{simdir}/{e_mu_dir}/an-res/ND280_e-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
  f"{simdir}/{e_mu_dir}/an-res/ND280_mu-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
  f"{simdir}/{e_mu_dir}/an-res/ND280_mu-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
  f"{simdir}/{ae_amu_dir}/an-res/ND280_e+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
  f"{simdir}/{ae_amu_dir}/an-res/ND280_e+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
  f"{simdir}/{ae_amu_dir}/an-res/ND280_mu+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
  f"{simdir}/{ae_amu_dir}/an-res/ND280_mu+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500/main-edep-data.csv",
]

# data = [
#   f"MDisplayApp {simdir}/{e_mu_dir}/root-sim/ND280_e-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500.root {simdir}/{e_mu_dir}/an-res/ND280_e-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500",
#   f"MDisplayApp {simdir}/{e_mu_dir}/root-sim/ND280_e-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500.root {simdir}/{e_mu_dir}/an-res/ND280_e-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500",
#   f"MDisplayApp {simdir}/{e_mu_dir}/root-sim/ND280_mu-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500.root {simdir}/{e_mu_dir}/an-res/ND280_mu-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500",
#   f"MDisplayApp {simdir}/{e_mu_dir}/root-sim/ND280_mu-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500.root {simdir}/{e_mu_dir}/an-res/ND280_mu-_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500",
#   f"MDisplayApp {simdir}/{ae_amu_dir}/root-sim/ND280_e+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500.root {simdir}/{ae_amu_dir}/an-res/ND280_e+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500",
#   f"MDisplayApp {simdir}/{ae_amu_dir}/root-sim/ND280_e+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500.root {simdir}/{ae_amu_dir}/an-res/ND280_e+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500",
#   f"MDisplayApp {simdir}/{ae_amu_dir}/root-sim/ND280_mu+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500.root {simdir}/{ae_amu_dir}/an-res/ND280_mu+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.000001y0z0utesla-Exp0-Nexpt500",
#   f"MDisplayApp {simdir}/{ae_amu_dir}/root-sim/ND280_mu+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500.root {simdir}/{ae_amu_dir}/an-res/ND280_mu+_E200uMeV_GPx0.5y0.5z0.0ucm_GDx0y0z1_Bx0.2y0z0utesla-Exp0-Nexpt500",
# ]

# for d in data:
#   print(d)

# exit()

for fl in files:
  path_parts = fl.split("/")
  directory = "/".join(path_parts[:-1])

  main_particle = []
  main_particle_range = []
  main_particle_edep = []
  main_particle_dedx_hits = []
  main_particle_dedx = []
  main_particle_has_elepos_pair = []
  with open(fl, "r") as f:
    lines = f.readlines()
    items = lines[0].split(",")
    for ln in lines[1:]:
      d = ln.split(",")
      d = [ float(it) if i != 1 else it for i, it in enumerate(d) ]
      main_particle.append(d[1])
      main_particle_range.append(d[2])
      main_particle_edep.append(d[3])
      main_particle_dedx_hits.append(d[4])
      main_particle_dedx.append(d[5])
      main_particle_has_elepos_pair.append(d[6])

  # main_particle = np.array(main_particle)
  main_particle = list(set(main_particle))[0]
  print(main_particle, fl)
  main_particle_range = np.array(main_particle_range)
  main_particle_edep = np.array(main_particle_edep)
  main_particle_dedx_hits = np.array(main_particle_dedx_hits)
  main_particle_dedx = np.array(main_particle_dedx)
  main_particle_has_elepos_pair = np.array(main_particle_has_elepos_pair)

  if main_particle in ["e+", "e-"]:
    main_particle_range_with_pair = main_particle_range[main_particle_has_elepos_pair == 1]
    main_particle_edep_with_pair = main_particle_edep[main_particle_has_elepos_pair == 1]
    main_particle_dedx_hits_with_pair = main_particle_dedx_hits[main_particle_has_elepos_pair == 1]
    main_particle_dedx_with_pair = main_particle_dedx[main_particle_has_elepos_pair == 1]

    main_particle_range_wo_pair = main_particle_range[main_particle_has_elepos_pair == 0]
    main_particle_edep_wo_pair = main_particle_edep[main_particle_has_elepos_pair == 0]
    main_particle_dedx_hits_wo_pair = main_particle_dedx_hits[main_particle_has_elepos_pair == 0]
    main_particle_dedx_wo_pair = main_particle_dedx[main_particle_has_elepos_pair == 0]

    # fig = plt.figure(figsize=(15,10))
    # Create a 2x2 subplot layout
    fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))

    fig.suptitle(f'{main_particle} 200 MeV, no gamma conversion in event', fontsize=16)

    # Plot data on each subplot
    # axes[0, 0].plot(x, y1, label='Sin(x)')
    axes[0, 0].hist(main_particle_range_wo_pair, range=(0.0, 150.0), bins=150)
    axes[0, 0].set_title(f'{main_particle} Range')
    axes[0, 0].set_xlabel(f'{main_particle} Range, cm')
    axes[0, 0].set_ylabel(f'N events')

    # axes[0, 1].plot(x, y2, label='Cos(x)', color='orange')
    axes[0, 1].hist(main_particle_edep_wo_pair, range=(0.0, 300), bins=100)
    axes[0, 1].set_title(f'{main_particle} total energy deposit')
    axes[0, 1].set_xlabel(f'{main_particle} total energy deposit, MeV')
    axes[0, 1].set_ylabel(f'N events')

    im = axes[1, 0].hist2d(main_particle_range_wo_pair, main_particle_dedx_wo_pair, range=[[0.0, 150.0], [0.0, 4.0]], bins=[150, 40])
    axes[1, 0].set_title(f'{main_particle} dE/dx, MeV/cm')
    axes[1, 0].set_xlabel(f'{main_particle} Range, cm')
    axes[1, 0].set_ylabel(f'dE/dx, MeV/cm')
    divider = make_axes_locatable(axes[1, 0])
    cax = divider.append_axes('right', size='5%', pad=0.05)
    # fig.colorbar(im, cax=cax, orientation='vertical')
    # cbar = fig.colorbar(im, ax=axes[1, 0], orientation='vertical')
    cbar = plt.colorbar(im[3], cax=cax)

    # axes[1, 1].hist(main_particle_dedx_hits_wo_pair, range=(0.0, 300), bins=100)
    # axes[1, 1].set_title(f'{main_particle} total energy deposit in hits')
    # axes[1, 1].set_xlabel(f'total energy deposit collected in hits, MeV')
    # axes[1, 1].set_ylabel(f'N events')
    axes[1, 1].remove()

    # Adjust layout to prevent clipping of titles
    plt.tight_layout()

    # Show the plot
    # plt.show()
    plt.savefig(f"{directory}/{main_particle}-energetic_props-nogammaconv.pdf")
    print(f"=={main_particle}: N events: ", main_particle_range_wo_pair.shape[0])
    print(f"=={main_particle}: np.mean(main_particle_range_wo_pair)", np.mean(main_particle_range_wo_pair))
    print(f"=={main_particle}: np.std(main_particle_range_wo_pair)", np.std(main_particle_range_wo_pair))

    ######
    fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))

    fig.suptitle(f'{main_particle} 200 MeV, gamma conversion in event', fontsize=16)

    # Plot data on each subplot
    # axes[0, 0].plot(x, y1, label='Sin(x)')
    axes[0, 0].hist(main_particle_range_with_pair, range=(0.0, 150.0), bins=150)
    axes[0, 0].set_title(f'{main_particle} Range')
    axes[0, 0].set_xlabel(f'{main_particle} Range, cm')
    axes[0, 0].set_ylabel(f'N events')

    # axes[0, 1].plot(x, y2, label='Cos(x)', color='orange')
    axes[0, 1].hist(main_particle_edep_with_pair, range=(0.0, 300), bins=100)
    axes[0, 1].set_title(f'{main_particle} total energy deposit')
    axes[0, 1].set_xlabel(f'{main_particle} total energy deposit, MeV')
    axes[0, 1].set_ylabel(f'N events')

    im = axes[1, 0].hist2d(main_particle_range_with_pair, main_particle_dedx_with_pair, range=[[0.0, 150.0], [0.0, 4.0]], bins=[150, 40])
    axes[1, 0].set_title(f'{main_particle} dE/dx, MeV/cm')
    axes[1, 0].set_xlabel(f'{main_particle} Range, cm')
    axes[1, 0].set_ylabel(f'dE/dx, MeV/cm')
    divider = make_axes_locatable(axes[1, 0])
    cax = divider.append_axes('right', size='5%', pad=0.05)
    # fig.colorbar(im, cax=cax, orientation='vertical')
    # cbar = fig.colorbar(im, ax=axes[1, 0], orientation='vertical')
    cbar = plt.colorbar(im[3], cax=cax)

    # axes[1, 1].hist(main_particle_dedx_hits_with_pair, range=(0.0, 300), bins=100)
    # axes[1, 1].set_title(f'{main_particle} total energy deposit in hits')
    # axes[1, 1].set_xlabel(f'total energy deposit collected in hits, MeV')
    # axes[1, 1].set_ylabel(f'N events')
    axes[1, 1].remove()

    # Adjust layout to prevent clipping of titles
    plt.tight_layout()

    # Show the plot
    # plt.show()
    plt.savefig(f"{directory}/{main_particle}-energetic_props-gammaconv.pdf")
    print(f"=={main_particle}: N events: ", main_particle_range_with_pair.shape[0])
    print(f"=={main_particle}: np.mean(main_particle_range_with_pair)", np.mean(main_particle_range_with_pair))
    print(f"=={main_particle}: np.std(main_particle_range_with_pair)", np.std(main_particle_range_with_pair))


    ######
    fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))

    fig.suptitle(f'{main_particle} 200 MeV', fontsize=16)

    # Plot data on each subplot
    # axes[0, 0].plot(x, y1, label='Sin(x)')
    axes[0, 0].hist(main_particle_range, range=(0.0, 150.0), bins=150)
    axes[0, 0].set_title(f'{main_particle} Range')
    axes[0, 0].set_xlabel(f'{main_particle} Range, cm')
    axes[0, 0].set_ylabel(f'N events')

    # axes[0, 1].plot(x, y2, label='Cos(x)', color='orange')
    axes[0, 1].hist(main_particle_edep, range=(0.0, 300), bins=100)
    axes[0, 1].set_title(f'{main_particle} total energy deposit')
    axes[0, 1].set_xlabel(f'{main_particle} total energy deposit, MeV')
    axes[0, 1].set_ylabel(f'N events')

    im = axes[1, 0].hist2d(main_particle_range, main_particle_dedx, range=[[0.0, 150.0], [0.0, 4.0]], bins=[150, 40])
    axes[1, 0].set_title(f'{main_particle} dE/dx, MeV/cm')
    axes[1, 0].set_xlabel(f'{main_particle} Range, cm')
    axes[1, 0].set_ylabel(f'dE/dx, MeV/cm')
    divider = make_axes_locatable(axes[1, 0])
    cax = divider.append_axes('right', size='5%', pad=0.05)
    # fig.colorbar(im, cax=cax, orientation='vertical')
    # cbar = fig.colorbar(im, ax=axes[1, 0], orientation='vertical')
    cbar = plt.colorbar(im[3], cax=cax)

    # axes[1, 1].hist(main_particle_dedx_hits, range=(0.0, 300), bins=100)
    # axes[1, 1].set_title(f'{main_particle} total energy deposit in hits')
    # axes[1, 1].set_xlabel(f'total energy deposit collected in hits, MeV')
    # axes[1, 1].set_ylabel(f'N events')
    axes[1, 1].remove()

    # Adjust layout to prevent clipping of titles
    plt.tight_layout()

    # Show the plot
    # plt.show()
    plt.savefig(f"{directory}/{main_particle}-energetic_props-all.pdf")
    print(f"=={main_particle}: N events: ", main_particle_range.shape[0])
    print(f"=={main_particle}: np.mean(main_particle_range)", np.mean(main_particle_range))
    print(f"=={main_particle}: np.std(main_particle_range)", np.std(main_particle_range))


  else:
    fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))

    fig.suptitle(f'{main_particle} 200 MeV', fontsize=16)

    # Plot data on each subplot
    # axes[0, 0].plot(x, y1, label='Sin(x)')
    axes[0, 0].hist(main_particle_range, range=(0.0, 150.0), bins=150)
    axes[0, 0].set_title(f'{main_particle} Range')
    axes[0, 0].set_xlabel(f'{main_particle} Range, cm')
    axes[0, 0].set_ylabel(f'N events')

    # axes[0, 1].plot(x, y2, label='Cos(x)', color='orange')
    axes[0, 1].hist(main_particle_edep, range=(0.0, 300), bins=100)
    axes[0, 1].set_title(f'{main_particle} total energy deposit')
    axes[0, 1].set_xlabel(f'{main_particle} total energy deposit, MeV')
    axes[0, 1].set_ylabel(f'N events')

    im = axes[1, 0].hist2d(main_particle_range, main_particle_dedx, range=[[0.0, 150.0], [0.0, 4.0]], bins=[150, 40])
    axes[1, 0].set_title(f'{main_particle} dE/dx, MeV/cm')
    axes[1, 0].set_xlabel(f'{main_particle} Range, cm')
    axes[1, 0].set_ylabel(f'dE/dx, MeV/cm')
    divider = make_axes_locatable(axes[1, 0])
    cax = divider.append_axes('right', size='5%', pad=0.05)
    # fig.colorbar(im, cax=cax, orientation='vertical')
    # cbar = fig.colorbar(im, ax=axes[1, 0], orientation='vertical')
    cbar = plt.colorbar(im[3], cax=cax)
    

    # axes[1, 1].hist(main_particle_dedx_hits, range=(0.0, 300), bins=100)
    # axes[1, 1].set_title(f'{main_particle} total energy deposit in hits')
    # axes[1, 1].set_xlabel(f'total energy deposit collected in hits, MeV')
    # axes[1, 1].set_ylabel(f'N events')
    axes[1, 1].remove()

    # Adjust layout to prevent clipping of titles
    plt.tight_layout()

    # Show the plot
    # plt.show()
    plt.savefig(f"{directory}/{main_particle}-energetic_props.pdf")
    print(f"=={main_particle}: N events: ", main_particle_range.shape[0])
    print(f"=={main_particle}: np.mean(main_particle_range)", np.mean(main_particle_range))
    print(f"=={main_particle}: np.std(main_particle_range)", np.std(main_particle_range))



  # plt.hist(main_particle_range, bins=100)
  # plt.show()

