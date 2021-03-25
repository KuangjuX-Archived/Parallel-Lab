./trans_1.sh && \
./trans_2.sh && \
./trans_3.sh && \
qsub trans_1.pbs && \
qsub trans_2.pbs && \
qsub trans_3.pbs 