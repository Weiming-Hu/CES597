# This script plots the OpenMPI profiling results from log*.txt files generated
# from the bash script profile-mpi-jacobi.sh
#

setwd('~/github/LinearSystemSolvers/scripts/report2')
log.files <- list.files(path = '.', pattern = 'log', full.names = T)
num.procs <- c(1, 2, 4, 8, 16, 32)
time.user <- matrix(-1, nrow = 5, ncol = length(num.procs))
time.real <- matrix(-1, nrow = 5, ncol = length(num.procs))
time.sys <- matrix(-1, nrow = 5, ncol = length(num.procs))

parallel_portion = 0.5
amdahl_x <- seq(min(num.procs), max(num.procs), length.out = 100)
amdahl_y <- 1 / (1 - parallel_portion + parallel_portion / amdahl_x)

for (i in 1:length(log.files)) {
  lines <- readLines(log.files[i])
  for (j in 1:length(num.procs)) {
    time.user[i, j] <- as.numeric(gsub('.*?(\\d+)m(\\d+\\.\\d+)s', '\\1', lines[3+(j-1)*4])) * 60 + as.numeric(gsub('.*m(\\d+\\.\\d+)s', '\\1', lines[3+(j-1)*4]))
    time.real[i, j] <- as.numeric(gsub('.*?(\\d+)m(\\d+\\.\\d+)s', '\\1', lines[2+(j-1)*4])) * 60 + as.numeric(gsub('.*m(\\d+\\.\\d+)s', '\\1', lines[2+(j-1)*4]))
    time.sys[i, j] <- as.numeric(gsub('.*?(\\d+)m(\\d+\\.\\d+)s', '\\1', lines[4+(j-1)*4])) * 60 + as.numeric(gsub('.*m(\\d+\\.\\d+)s', '\\1', lines[4+(j-1)*4]))
  }
}

library(reshape2)
library(RColorBrewer)
library(ggplot2)

# Table
user.vec <- colMeans(time.user)
real.vec <- colMeans(time.real)
sys.vec <- colMeans(time.sys)

# Amdahl
sp <- real.vec[1] / real.vec
ylim <- range(c(sp, amdahl_y))
plot(1:max(num.procs), type = 'n', ylim = ylim,
      xlab = '# of Processes', ylab = 'Speed Up')
lines(amdahl_x, amdahl_y, col = 'red', lwd = 2)
lines(num.procs, sp, col = 'green', lwd = 2)
legend('bottomleft', legend = c('Theory', 'MPI Jacobi'), lwd = 2, col = c('red', 'green'))

if (F) {
  png('amdahl.png', width = 7, height = 4, res = 300, units = 'in')
  par(mar = c(3, 3, 1, 1))
  plot(1:max(num.procs), type = 'n', ylim = ylim,
       xlab = '# of Processes', ylab = 'Speed Up')
  lines(amdahl_x, amdahl_y, col = 'red', lwd = 2)
  lines(num.procs, sp, col = 'green', lwd = 2)
  legend('bottomleft', legend = c('Theory', 'MPI Jacobi'), lwd = 2, col = c('red', 'green'))
  dev.off()
}