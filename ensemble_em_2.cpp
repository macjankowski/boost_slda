//
//  ensemble_em.cpp
//  sLdaEnsemble
//
//  Created by Maciej Jankowski on 29/12/2017.
//  Copyright © 2017 Maciej Jankowski. All rights reserved.
//

#include "ensemble_em_2.h"
#include "ensemble_em.h"
#include <stdexcept>
#include "utils.h"

void save_model_weight(const char * directory, double model_weight)
{
    char filename[100];
    sprintf(filename, "%s/model_weight_in_ensemble.txt", directory);
    FILE * file = NULL;
    file = fopen(filename, "w");
    fprintf(file, "%lf\n", model_weight);
    fflush(file);
    fclose(file);
}

void train_ensemble_2(double alpha, corpus * c, const settings * setting, char * directory, int num_iter, int topics_numer[], int L){

    //const double Epsilon = 0.000001;
    
    //const int NUM_ITER = 100;
    const int M = c->num_docs;
    char init_method[200];
    char model_directory[200];
    
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
    
    sprintf(init_method, "%s", "seeded");
    
    
    for(int j=0; j<num_iter; j++){
    
        for(int i=0; i<L; i++){
            
            
//            sprintf(model_directory, "%s/%d", directory,topics_numer[i]);
            sprintf(model_directory, "%s/%d", directory,j*L+i);
            make_directory(model_directory);
            
//            if(j > 0){
//                sprintf(init_method, "%s/%d", directory,j-1);
//            }
            
            printf("Init method: %s\n",init_method);
            
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
            save_model_weight(model_directory,model_weights[i]);
            
            recalculate_weights(w, M, model_weights[i], classified);
            //recalculate_weights(w, M, model_weights[i], classified);
            
        }
    }
    
}



void infer_all(corpus * c, const settings * setting, char * root_out_directory, char * root_model_directory, int num_iter, int topics_numer[], int L){
    
    //const double Epsilon = 0.000001;
    
    //const int NUM_ITER = 100;
    const int M = c->num_docs;
    //const int L = 1;
    //const char * init_method = "seeded";
    char out_directory[200];
    char model_filename[200];
    
    //int topics_numer[L] = {5,10,15,20,30};
    //int topics_numer[L] = {15};
    
    for(int j=0; j<num_iter; j++){
        
        for(int i=0; i<L; i++){
        
        sprintf(out_directory, "%s/%d", root_out_directory,j*L+i);
        make_directory(out_directory);
        printf("Inferrence output path %s\n",out_directory);
        
        sprintf(model_filename, "%s/%d/final.model", root_model_directory,j*L+i);
        printf("Inferrence using model %s\n",model_filename);
        
        slda model;
        model.load_model(model_filename);
        //model.infer_only(c, setting, out_directory);
        
        classification classified = model.calculate_prob_of_correct(setting, c, out_directory);
        
        }
    }
    
    
    
}





























