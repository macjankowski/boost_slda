
Written by Maciej Jankowski, maciej.jankowski@wat.edu.pl, most of code is from https://github.com/blei-lab/class-slda

This is a C++ implementation of Boost Multi-class sLDA, the extension of class-slda that works in ensemble of clissifiers. The details are described in Boost multi-class slda model for text classication, 01 2018. Artificial Intelligence and Soft Computing - 17th International Conference, ICAISC 2018, Zakopane, Poland, June 3-7, 2018, Proceedings, Part I.

Example training

./slda est sample-data/smsSpam/smsSpam.documents.txt sample-data/smsSpam/smsSpam.isSpam.txt settings.txt 0.1 seeded results/train_test_2 3 5,10,15,20,30

where 

est - estimate parameters
sample-data/smsSpam/smsSpam.documents.txt - training data
sample-data/smsSpam/smsSpam.isSpam.txt - train labels
settings.txt - file with settings (as described in README_ORIG.md)
0.1 - as described in README_ORIG.md 
seeded - as described in README_ORIG.md
results/train_test_2 - output folder with models. Each model is in format as described in README_ORIG.md
3 - number of full iteration. Single iteration trains all models 
5,10,15,20,30 - topic numbers for trained models


Example Inference

./slda inf sample-data/smsSpam/smsSpamTest.documents.txt sample-data/smsSpam/smsSpamTest.isSpam.txt settings.txt results/train_test_2 results/infer_test_2 3 5,10,15,20,30

where
inf - inference phase
sample-data/smsSpam/smsSpamTest.documents.txt - test data 
sample-data/smsSpam/smsSpamTest.isSpam.txt - test labels
settings.txt - as described in README_ORIG.md
results/train_test_2 - folder with models
results/infer_test_2 - output folder with results of inference
3 - number of full iteration 
5,10,15,20,30 - topic numbers for trained models

