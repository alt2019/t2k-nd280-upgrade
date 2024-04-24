/**
 * @file EventDisplay.hh
 *
 * @brief TODO
 *
 * @creation_date     { 23.03.2024 }
 * @modification_date ( 23.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef EVENT_DISPLAY_H
#define EVENT_DISPLAY_H

#include <TH1F.h>
#include <TH2F.h>

class EventDataDrawing
{
  public:
    TH1F * h_n_vtx_out_tracks;
    TH1F * h_n_vtx_out_protons;
    TH1F * h_n_vtx_out_neutrons;
    TH1F * h_n_vtx_out_pos_pions;
    TH1F * h_n_vtx_out_neg_pions;
    TH1F * h_n_vtx_out_neu_pions;

    EventDataDrawing();
    ~EventDataDrawing();
  private:

};

#endif
