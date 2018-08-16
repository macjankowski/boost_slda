CC = g++
LDFLAGS = -lgsl -lm -lgslcblas


LSOURCE = main.cpp corpus.cpp slda.cpp utils.cpp opt.cpp ensemble_em.cpp ensemble_em_2.cpp
LHEADER = corpus.h slda.h utils.h opt.h settings.h ensemble_em.h ensemble_em_2.h

slda: $(LSOURCE) $(HEADER)
	  $(CC) $(LSOURCE) -o $@ $(LDFLAGS)

clean:
	-rm -f *.o slda
