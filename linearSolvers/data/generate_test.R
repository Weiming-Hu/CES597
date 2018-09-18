# "`-''-/").___..--''"`-._
#  (`6_ 6  )   `-.  (     ).`-.__.`)   WE ARE ...
#  (_Y_.)'  ._   )  `._ `. ``-..-'    PENN STATE!
#    _ ..`--'_..-_/  /--'_.' ,'
#  (il),-''  (li),'  ((!.-'
# 
# Author: Weiming Hu <weiming@psu.edu>
#         Geoinformatics and Earth Observation Laboratory (http://geolab.psu.edu)
#         Department of Geography and Institute for CyberScience
#         The Pennsylvania State University
#
# This script is created to generate a test data set based on the input size.
# Test matrix will be stored in A_<size>.csv.
# Test vector will be stored in b_<size>.csv.
# Solution vector will be stored in x_<size>.csv.
# File names will be the size as suffix following an underscore.
#

# size <- readline("Size: ")
size <- 10

A <- matrix(sample(size*size)/size, nrow = size, ncol = size)
b <- sample(size)
x <- solve(A, b)

cat(paste("Generate test files for size", size, "..."))
write.table(A, file = paste("A_", size, ".csv", sep = ''),
           quote = F, row.names = F, col.names = F, sep = ',')
write.table(b, file = paste("b_", size, ".csv", sep = ''),
           quote = F, row.names = F, col.names = F, sep = ',')
write.table(x, file = paste("x_", size, ".csv", sep = ''),
           quote = F, row.names = F, col.names = F, sep = ',')
cat(" done!\n")