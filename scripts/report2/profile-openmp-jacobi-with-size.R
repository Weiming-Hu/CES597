# This script profiles the OpenMP implementation of Jacobi iterative method
# using data sets with a specified size.
#

cat("Change the working directory to ~/github/LinearSystemSolvers/scripts/report2/\n")
setwd("~/github/LinearSystemSolvers/scripts/report2/")

output.data.file <- 'profile-openmp-jacobi-with-size.RData'
# data.sizes <- c(50, 100, 150, 200, 250, 300, 350, 400)
data.size <- 250
repeat.times <- 5
num.cores <- c(1, 2, 4, 8, 16, 32)

if (!file.exists(output.data.file)) {
  
  cat('Output data file does not exist. Generate one ...\n')
  exe <- '../../output/bin/iterativeSolver'
  data.folder <- '../../data/'
  solver.type <- 'J'
  initialization <- 3
  iterations <- 7000
  verbose <- 0
  
  profile.list <- list()
  profile.list$matirxInversion = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$forwardElimination = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$backwardElimination = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$preprocessing = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$loop = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$postprocessing = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$jacobi = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$readingData = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$iterativeMethod = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$total = matrix(nrow = repeat.times, ncol = length(num.cores))
  
  profile.list$wall_preprocessing = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$wall_loop = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$wall_postprocessing = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$wall_jacobi = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$wall_readingData = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$wall_iterativeMethod = matrix(nrow = repeat.times, ncol = length(num.cores))
  profile.list$wall_total = matrix(nrow = repeat.times, ncol = length(num.cores))
  
  for (index in 1:repeat.times) {
    cat(paste('Repeat #', index, 'out of', repeat.times, '\n'))
    
    for (i in 1:length(num.cores)) {
      cat(paste('Working on event with cores index #', i, 'out of', length(num.cores), '\n'))
      command <- paste('OMP_NUM_THREADS=', num.cores[i], ' ', exe, ' ', solver.type, ' ',
                       data.folder, 'A_', data.size, '.csv ',
                       data.folder, 'b_', data.size, '.csv ', iterations,
                       ' ', initialization, ' ', verbose, sep = '')
      ret <- system(command, intern = T)
      
      if (length(ret) != 20) {
        stop(paste('Error: repeat #', index, 'with data size #', i, 'has a problem.\n'))
      }
      
      profile.list$forwardElimination[index, i] = as.numeric(gsub('Forward elimination: (.+)s .*', '\\1', ret[[3]]))
      profile.list$backwardElimination[index, i] = as.numeric(gsub('Backward elimination: (.+)s .*', '\\1', ret[[4]]))
      profile.list$matirxInversion[index, i] = as.numeric(gsub('Inverse function total: (.+)s .*', '\\1', ret[[5]]))
      profile.list$preprocessing[index, i] = as.numeric(gsub('\\(Jacobi\\) Preprocessing: (.+)s .*', '\\1', ret[[7]]))
      profile.list$loop[index, i] = as.numeric(gsub('\\(Jacobi\\) Loop: (.+)s .*', '\\1', ret[[8]]))
      profile.list$postprocessing[index, i] = as.numeric(gsub('\\(Jacobi\\) Postprocessing: (.+)s .*', '\\1', ret[[9]]))
      profile.list$jacobi[index, i] = as.numeric(gsub('\\(Jacobi\\) Total time: (.+)s .*', '\\1', ret[[10]]))
      
      profile.list$wall_preprocessing[index, i] = as.numeric(gsub('\\(Jacobi\\) Wall time Preprocessing: (.+)s .*', '\\1', ret[[11]]))
      profile.list$wall_loop[index, i] = as.numeric(gsub('\\(Jacobi\\) Wall time Loop: (.+)s .*', '\\1', ret[[12]]))
      profile.list$wall_postprocessing[index, i] = as.numeric(gsub('\\(Jacobi\\) Wall time Postprocessing: (.+)s .*', '\\1', ret[[13]]))
      profile.list$wall_jacobi[index, i] = as.numeric(gsub('\\(Jacobi\\) Wall time Total time: (.+)s .*', '\\1', ret[[14]]))
      
      profile.list$readingData[index, i] = as.numeric(gsub('Reading data: (.+)s .*', '\\1', ret[[15]]))
      profile.list$iterativeMethod[index, i] = as.numeric(gsub('Iterative method: (.+)s .*', '\\1', ret[[16]]))
      profile.list$total[index, i] = as.numeric(gsub('Total time: (.+)s .*', '\\1', ret[[17]]))
      
      profile.list$wall_readingData[index, i] = as.numeric(gsub('Wall time Reading data: (.+)s .*', '\\1', ret[[18]]))
      profile.list$wall_iterativeMethod[index, i] = as.numeric(gsub('Wall time Iterative method: (.+)s .*', '\\1', ret[[19]]))
      profile.list$wall_total[index, i] = as.numeric(gsub('Wall time Total time: (.+)s .*', '\\1', ret[[20]]))
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
library(tidyverse)

melted_wall <- matrix(nrow = 0, ncol = 3)
for (name in names(profile.list)[c(11, 12, 13, 15)]) {
  melted_wall <- rbind(melted_wall, cbind(melt(profile.list[[name]])[, c(2, 3)], name))
}
melted_wall[, 1] <- as.factor(num.cores[melted_wall[, 1]])
melted_wall[, 2] <- melted_wall[, 2] / repeat.times
melted_wall[, 3] <- as.factor(melted_wall[, 3])
melted_wall <- cbind(melted_wall, 'Wall')
colnames(melted_wall) <- c('num.cores', 'value', 'Routines', 'clock.type')

melted_cpu <- matrix(nrow = 0, ncol = 3)
for (name in names(profile.list)[c(4, 5, 6, 8)]) {
  melted_cpu <- rbind(melted_cpu, cbind(melt(profile.list[[name]])[, c(2, 3)], name))
}
melted_cpu[, 1] <- as.factor(num.cores[melted_cpu[, 1]])
melted_cpu[, 2] <- melted_cpu[, 2] / repeat.times
melted_cpu[, 3] <- as.factor(melted_cpu[, 3])
melted_cpu <- cbind(melted_cpu, 'CPU')
colnames(melted_cpu) <- c('num.cores', 'value', 'Routines', 'clock.type')

melted_all <- rbind(melted_cpu, melted_wall)
melted_all[, 4] <- as.factor(melted_all[, 4])
melted_all <- data.frame(melted_all) %>% 
  mutate(x_label = factor(str_replace(interaction(str_pad(
    num.cores, width = 3), clock.type), '\\.', ' / '), ordered = TRUE))

g <- ggplot(melted_all, aes(fill = Routines, y = value, x = x_label)) + 
  xlab('Number of Cores / Clock Type') + ylab('Time (s)') +
  geom_bar(stat="identity", width = 0.7) + 
  # scale_color_brewer('Paired') +
  scale_colour_brewer(palette = "Greens") +
  # facet_wrap( ~ clock.type) +
  theme(legend.background = element_rect(fill=alpha('white', 0)),
        legend.position = c(.15, .8),
        text = element_text(size=18)) +
  guides(color = guide_legend(title = "Routines"))

if (F) {
  png('profile-openmp-jacobi-with-size.png',
      width = 11, height = 7, res = 300, units = 'in')
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