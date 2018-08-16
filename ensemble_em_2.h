//
//  ensemble_em.hpp
//  sLdaEnsemble
//
//  Created by Maciej Jankowski on 29/12/2017.
//  Copyright © 2017 Maciej Jankowski. All rights reserved.
//

#ifndef ensemble_em_2_hpp
#define ensemble_em_2_hpp

#include <stdio.h>
#include "corpus.h"
#include "utils.h"
#include "slda.h"


void train_ensemble_2(double alpha, corpus * c, const settings * setting, char * directory, int num_iter, int *topic_numbers, int models_count);

void save_model_weight(const char * directory, double model_weight);

void infer_all(corpus * c, const settings * setting, char * root_out_directory, char * root_model_directory, int num_iter, int topics_numer[], int L);

#endif /* ensemble_em_2_hpp */
