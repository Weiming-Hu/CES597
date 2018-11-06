# This script profiles the serial implementation of Jacobi iterative method
# using data sets with different sizes.
#

cat("Change the working directory to ~/github/LinearSystemSolvers/scripts/report2/\n")
setwd("~/github/LinearSystemSolvers/scripts/report2/")

output.data.file <- 'profile-serial-jacobi-with-sizes.RData'
data.sizes <- c(50, 100, 150, 200, 250, 300, 350, 400)
repeat.times <- 10

if (!file.exists(output.data.file)) {
  
  cat('Output data file does not exist. Generate one ...\n')
  exe <- '../../output/bin/iterativeSolver'
  data.folder <- '../../data/'
  solver.type <- 'J'
  initialization <- 3
  iterations <- 7000
  verbose <- 0
  num.cores <- 1
  
  profile.list <- list()
  profile.list$matirxInversion = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$forwardElimination = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$backwardElimination = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$preprocessing = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$loop = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$postprocessing = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$jacobi = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$readingData = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$iterativeMethod = matrix(nrow = repeat.times, ncol = length(data.sizes))
  profile.list$total = matrix(nrow = repeat.times, ncol = length(data.sizes))
  
  for (index in 1:repeat.times) {
    cat(paste('Repeat #', index, 'out of', repeat.times, '\n'))
    
    for (i in 1:length(data.sizes)) {
      cat(paste('Working on data size #', i, 'out of', length(data.sizes), '\n'))
      command <- paste('OMP_NUM_THREADS=', num.cores, ' ', exe, ' ', solver.type, ' ',
                       data.folder, 'A_', data.sizes[i], '.csv ',
                       data.folder, 'b_', data.sizes[i], '.csv ', iterations,
                       ' ', initialization, ' ', verbose, sep = '')
      ret <- system(command, intern = T)
      
      if (length(ret) != 13) {
        stop(paste('Error: repeat #', index, 'with data size #', i, 'has a problem.\n'))
      }
      
      profile.list$forwardElimination[index, i] = as.numeric(gsub('Forward elimination: (.+)s .*', '\\1', ret[[3]]))
      profile.list$backwardElimination[index, i] = as.numeric(gsub('Backward elimination: (.+)s .*', '\\1', ret[[4]]))
      profile.list$matirxInversion[index, i] = as.numeric(gsub('Inverse function total: (.+)s .*', '\\1', ret[[5]]))
      profile.list$preprocessing[index, i] = as.numeric(gsub('\\(Jacobi\\) Preprocessing: (.+)s .*', '\\1', ret[[7]]))
      profile.list$loop[index, i] = as.numeric(gsub('\\(Jacobi\\) Loop: (.+)s .*', '\\1', ret[[8]]))
      profile.list$postprocessing[index, i] = as.numeric(gsub('\\(Jacobi\\) Postprocessing: (.+)s .*', '\\1', ret[[9]]))
      profile.list$jacobi[index, i] = as.numeric(gsub('\\(Jacobi\\) Total time: (.+)s .*', '\\1', ret[[10]]))
      profile.list$readingData[index, i] = as.numeric(gsub('Reading data: (.+)s .*', '\\1', ret[[11]]))
      profile.list$iterativeMethod[index, i] = as.numeric(gsub('Iterative method: (.+)s .*', '\\1', ret[[12]]))
      profile.list$total[index, i] = as.numeric(gsub('Total time: (.+)s .*', '\\1', ret[[13]]))
    }
  }
  
  save(profile.list, file = output.data.file)
} else {
  cat("Reading data file ... \n")
  load(output.data.file)
}

library(reshape2)
library(RColorBrewer)
library(ggplot2)

melted <- matrix(nrow = 0, ncol = 3)
for (name in names(profile.list)[c(4, 5, 6, 8)]) {
  melted <- rbind(melted, cbind(melt(profile.list[[name]])[, c(2, 3)], name))
}
melted[, 1] <- as.factor(data.sizes[melted[, 1]])
melted[, 2] <- melted[, 2] / repeat.times
melted[, 3] <- as.factor(melted[, 3])
colnames(melted) <- c('data.size', 'value', 'Routines')

g <- ggplot(melted, aes(fill = Routines, y = value, x = data.size)) + 
  xlab('Data Size') + ylab('Time Elapsed (s)') +
  scale_color_brewer('Dark2') +
  geom_bar( stat="identity") + 
  theme(legend.background = element_rect(fill=alpha('white', 0)),
        legend.position = c(.2, .8),
        text = element_text(size=20)) +
  guides(color = guide_legend(title = "Routines"))

if (F) {
  png('profile-serial-jacobi-with-sizes.png',
      width = 8, height = 6, res = 300, units = 'in')
  plot(g)
  dev.off()
}


# col.means <- lapply(profile.list, function(l) {return(colMeans(l))})
# data <- matrix(unlist(col.means), nrow = length(col.means), byrow = T)
# 
# plot(0, 0, type = 'n', xlim = c(1, ncol(data)), ylim = range(data),
#      xlab = 'Data Size', ylab = 'Time Elapsed (s)', xaxt = 'n')
# axis(1, at = 1:5, labels = data.sizes)
# cols <- colorRampPalette(brewer.pal(11, 'Spectral')[11:1])(nrow(data))
# for (i in 1:nrow(data)) {
#   lines(1:ncol(data), data[i, ], col = cols[i])
# }