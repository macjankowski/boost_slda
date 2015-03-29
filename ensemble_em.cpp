//
//  ensemble_em.cpp
//  sLdaEnsemble
//
//  Created by Maciej Jankowski on 29/12/2017.
//  Copyright © 2017 Maciej Jankowski. All rights reserved.
//

#include "ensemble_em.h"
#include <stdexcept>
#include "utils.h"

void train_ensemble(double alpha, corpus * c, const settings * setting, char * directory){
    
    //const double Epsilon = 0.000001;
    
    
    const char * init_method = "seeded";
    char model_directory[100];
    
    int length = 3;
    int topics_numer[3] = {2,3,4};
    double pi[3] = {0.33,0.33,0.34};
    double ** r = new double * [3];
    double *prob_of_correct[3];
    double sum_prob_of_correct[773];
    vector<classification> classifications;
    
    for(int i=0; i<length; i++){
        r[i] = new double[773];
    }
    
    
    for(int i=0; i<length; i++){
        
        sprintf(model_directory, "%s/%d", directory,topics_numer[i]);
        make_directory(model_directory);
        
        slda model;
        model.init(alpha, topics_numer[i], c);
        model.v_em(c, setting, init_method, model_directory);
        
        classification classified = model.calculate_prob_of_correct(setting, c, model_directory);
        classifications.push_back(classified);
        
        printf("Fitted model with K=%d\n", topics_numer[i]);
    }
    
    for(int n=0; n<c->num_docs; n++){
        sum_prob_of_correct[n] = 0;
        for(int i=0; i<length;i++){
            double prob = classifications[i].predictions[n]->probability;
            sum_prob_of_correct[n] += pi[i]*prob;
        }
        //printf("sum_prob_of_correct[%d] = %f\n",n,sum_prob_of_correct[n]);
    }
    
    for(int i=0; i<length; i++){
        for(int n=0; n<c->num_docs; n++){
            double prob = classifications[i].predictions[n]->probability;
            r[i][n] = (prob * pi[i])/sum_prob_of_correct[n];
        }
    }

    
    double ** r_t = transpose(r, length, c->num_docs);
    
    int num_correct = 0;
    for(int n=0; n<c->num_docs; n++){
        int winner = argmax(r_t[n], length);
        double predicted_label_of_winner = classifications[winner].predictions[n]->label;
        
        document * doc = c->docs[n];
        if (predicted_label_of_winner == doc->label)
            num_correct ++;
    }
    
    printf("average accuracy of ensemble: %.3f\n", (double)num_correct / (double) c->num_docs);
    
}


