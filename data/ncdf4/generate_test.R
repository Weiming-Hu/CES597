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
#
# This scripts geneartes test data files in NetCDF4 format.
#
# Test matrix is stored in the variable A.
# Test vector is stored in the variable b.
# Solution vector is stored in the variable x.
# File names are the size of the problem.

library(ncdf4)
it <- 1
size <- 100

while (it < 8) {
  A <- matrix(sample(size*size)/size, nrow = size, ncol = size)
  b <- sample(size)
  
  # Make the matrix diagonally dominant
  for (i in 1:nrow(A)) {
    A[i, i] <- sum(abs(A[i, ])) + abs(A[i, i])
  }
  
  x <- solve(A, b)
  
  dim <- ncdim_def('size', '', 1:size, create_dimvar = F)
  var.A <- ncvar_def('A', '', list(dim, dim), prec = 'double')
  var.b <- ncvar_def('b', '', list(dim), prec = 'double')
  var.x <- ncvar_def('x', '', list(dim), prec = 'double')
  
  file.name <- paste(size, '.nc', sep = '')
  unlink(file.name)
  nc <- nc_create(file.name, list(var.x, var.b, var.A), force_v4 = T)
  ncvar_put(nc, var.A, A)
  ncvar_put(nc, var.x, x)
  ncvar_put(nc, var.b, b)
  nc_close(nc)
  
  size <- size * 2
  it <- it + 1
}
