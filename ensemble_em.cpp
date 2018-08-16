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

void init_model_weights(double *doc_weights, int length){
    for(int i=0; i<length; i++){
        doc_weights[i] = 1.0/((double)length);
    }
}

void recalculate_weights(double *doc_weights, int length, double model_weight, classification classified){
    for(int i=0; i<length; i++){
        int prediction = classified.predictions[i]->predicted_label;
        int true_label =classified.predictions[i]->true_label;
        
        int incorrect;
        if(prediction != true_label){
            incorrect = 1;
        }else{
            incorrect = 0;
        }
        doc_weights[i] = doc_weights[i] * exp(model_weight * incorrect);
    }
}

void train_ensemble(double alpha, corpus * c, const settings * setting, char * directory){
    
    //const double Epsilon = 0.000001;
    
    const int NUM_ITER = 1;
    const int M = c->num_docs;
    const int L = 4;
    const char * init_method = "seeded";
    char model_directory[200];
    
    int topics_numer[L] = {2,3,5,10};
    //double pi[L] = {0.33,0.33,0.34};
    double ** r = new double * [L];
    
    double * w = new double[M];
    double *model_weights = new double[M];
    double *prob_of_correct[L];
    double sum_prob_of_correct[M];
    vector<classification> classifications;
    
    for(int i=0; i<L; i++){
        r[i] = new double[M];
    }
    
    // init weights for points
    init_model_weights(w, M);
    
    
    for(int j=0; j<NUM_ITER; j++){
    
        for(int i=0; i<L; i++){
            
            //        for(int n=0; n < c->num_docs; n++){
            //            printf("%.6f\t",w[n]);
            //        }
            
            sprintf(model_directory, "%s/%d", directory,topics_numer[i]);
            make_directory(model_directory);
            
            slda model;
            model.init(alpha, topics_numer[i], c);
            model.v_em(c, setting, init_method, model_directory, w);
            
            classification classified = model.calculate_prob_of_correct(setting, c, model_directory);
            classifications.push_back(classified);
            
            printf("Fitted model with K=%d\n", topics_numer[i]);
            
            double model_error = classified.weighted_error(w, M);
            printf("Model weighted error %.6f\n",model_error);
            
            model_weights[i] = log((1-model_error)/model_error);
            printf("Model weight %.6f\n",model_weights[i]);
            
            recalculate_weights(w, M, model_weights[i], classified);
            //recalculate_weights(w, M, model_weights[i], classified);
            
        }
    }
    
    
    
    for(int n=0; n<c->num_docs; n++){
        sum_prob_of_correct[n] = 0;
        for(int i=0; i<L;i++){
            double prob = classifications[i].predictions[n]->probability_of_predicted_label;
            //double prob = classifications[i].predictions[n]->probability_of_true_label;
            sum_prob_of_correct[n] += model_weights[i]*prob;
            //sum_prob_of_correct[n] += prob;
        }
        //printf("sum_prob_of_correct[%d] = %f\n",n,sum_prob_of_correct[n]);
    }
    
    for(int i=0; i<L; i++){
        for(int n=0; n<c->num_docs; n++){
            double prob = classifications[i].predictions[n]->probability_of_predicted_label;
            //double prob = classifications[i].predictions[n]->probability_of_true_label;
            r[i][n] = (prob * model_weights[i])/sum_prob_of_correct[n];
            //r[i][n] = prob/sum_prob_of_correct[n];
        }
    }
    
    //double *weighted_score_for_class =
    
    // choose class that
    double ** r_t = transpose(r, L, c->num_docs);
    
    int num_correct = 0;
    for(int n=0; n<c->num_docs; n++){
        int winner = argmax(r_t[n], L);
        double predicted_label_of_winner = classifications[winner].predictions[n]->predicted_label;
        
        document * doc = c->docs[n];
        if (predicted_label_of_winner == doc->label)
            num_correct ++;
    }
    
    printf("average accuracy of ensemble: %.3f\n", (double)num_correct / (double) c->num_docs);
    
}


