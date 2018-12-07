parallel_portion = 0.95
amdahl_x <- seq(1, 64, length.out = 100)
# amdahl_x <- as.numeric(colnames(fix.size.prog))
amdahl_y <- 1 / (1 - parallel_portion + parallel_portion / amdahl_x)

if (T) {
  setwd('~/github/LinearSystemSolvers/scripts/final/')
  
  wd.folder <- '~/github/LinearSystemSolvers/scripts/final/profile1/'
  source('read-profile.R')
  setwd('~/github/LinearSystemSolvers/scripts/final/')
  
  # boxplot(fix.np.prog)
  # boxplot(fix.np.time)
  
  boxplot(fix.size.prog)
  boxplot(fix.size.time)
  fix.size.time1 <- fix.size.time
  
  sp <- colMeans(fix.size.time[, 1] / fix.size.time)
  ylim <- range(c(sp, amdahl_y))
  plot(1:64, type = 'n', ylim = ylim,
       xlab = '# of Processes', ylab = 'Speed Up')
  lines(amdahl_x, amdahl_y, col = 'red', lwd = 2)
  lines(as.numeric(names(sp)), sp, col = 'green', lwd = 2)
  legend('topleft', legend = c('Theory', 'MPI Jacobi'), lwd = 2, col = c('red', 'green'))
  
}

if (T) {
  setwd('~/github/LinearSystemSolvers/scripts/final/')
  
  wd.folder <- '~/github/LinearSystemSolvers/scripts/final/profile2/'
  source('read-profile.R')
  setwd('~/github/LinearSystemSolvers/scripts/final/')
  
  # boxplot(fix.np.prog)
  # boxplot(fix.np.time)
  
  boxplot(fix.size.prog)
  boxplot(fix.size.time)
  
  fix.size.time2 <- fix.size.time
  
  sp1 <- colMeans(fix.size.time[, 1] / fix.size.time)
  ylim <- range(c(sp1, amdahl_y))
  plot(1:64, type = 'n', ylim = ylim,
       xlab = '# of Processes', ylab = 'Speed Up')
  lines(amdahl_x, amdahl_y, col = 'red', lwd = 2)
  lines(as.numeric(names(sp1)), sp1, col = 'green', lwd = 2)
  legend('topleft', legend = c('Theory', 'MPI Jacobi'), lwd = 2, col = c('red', 'green'))
  
}

if (T) {
  setwd('~/github/LinearSystemSolvers/scripts/final/')
  
  wd.folder <- '~/github/LinearSystemSolvers/scripts/final/profile3/'
  source('read-profile.R')
  setwd('~/github/LinearSystemSolvers/scripts/final/')
  
  # boxplot(fix.np.prog)
  # boxplot(fix.np.time)
  
  boxplot(fix.size.prog)
  boxplot(fix.size.time)
  
  fix.size.time3 <- fix.size.time
  
  sp2 <- colMeans(fix.size.time[, 1] / fix.size.time)
  ylim <- range(c(sp2, amdahl_y))
  plot(1:64, type = 'n', ylim = ylim,
       xlab = '# of Processes', ylab = 'Speed Up')
  lines(amdahl_x, amdahl_y, col = 'red', lwd = 2)
  lines(as.numeric(names(sp2)), sp2, col = 'green', lwd = 2)
  legend('topleft', legend = c('Theory', 'MPI Jacobi'), lwd = 2, col = c('red', 'green'))
}

if (T) {
  library(ggplot2)
  library(reshape2)
  melt1 <- melt(fix.size.time1)
  melt1$Var1 <- '800'
  
  melt2 <- melt(fix.size.time2)
  melt2$Var1 <- '1600'
  
  melt3 <- melt(fix.size.time3)
  melt3$Var1 <- '3200'
  
  df <- rbind(melt1, melt2, melt3)
  names(df) <- c('Size', 'Processes', 'Time')
  
  df$Size <- factor(df$Size)
  df$Processes <- factor(df$Processes)
  
  p <- ggplot(df, aes(x = Processes, col = Size, y = Time)) +
    geom_boxplot() + ylab('Time (s)')
    theme(text = element_text(size=20))
  png(filename = 'figures/fix-size-time.png', width = 10, height = 7, res = 400, units = 'in')
  plot(p)
  dev.off()
}

if (T) {
  ylim <- range(c(sp, sp1, sp2, amdahl_y))
  
  png('figures/fix-size-time-amdahl.png', width = 9, height = 8, res = 400, units = 'in')
  plot(1:64, type = 'n', ylim = ylim,
       xlab = '# of Processes', ylab = 'Speed Up')
  lines(amdahl_x, amdahl_y, col = 'red', lty = 'dashed', lwd = 2)
  lines(as.numeric(names(sp)), sp, col = 'green', type = 'b', pch = 3, lwd = 2)
  lines(as.numeric(names(sp)), sp1, col = 'green', type = 'b', pch = 1, lwd = 2)
  lines(as.numeric(names(sp)), sp2, col = 'green', type = 'b', pch = 2, lwd = 2)
  legend('topleft', legend = c('Theory', 'size 800*800', 'size 1600*1600', 'size 3200*3200'), lwd = 2,
         col = c('red', 'green', 'green', 'green'), lty = c('dashed', 'solid', 'solid', 'solid'),
         pch = c(-1, 3, 1, 2))
  dev.off()
}
