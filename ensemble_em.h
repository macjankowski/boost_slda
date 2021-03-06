//
//  ensemble_em.hpp
//  sLdaEnsemble
//
//  Created by Maciej Jankowski on 29/12/2017.
//  Copyright © 2017 Maciej Jankowski. All rights reserved.
//

#ifndef ensemble_em_hpp
#define ensemble_em_hpp

#include <stdio.h>
#include "corpus.h"
#include "utils.h"
#include "slda.h"

void train_ensemble(double alpha, corpus * c, const settings * setting, char * directory);


double * calculate_prob_of_correct(const settings * setting, corpus * corp, const char * directory, slda model);

void init_model_weights(double *weights, int length);

void recalculate_weights(double *doc_weights, int length, double model_weight, classification classified);

#endif /* ensemble_em_hpp */
