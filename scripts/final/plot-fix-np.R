if (T) {
  setwd('~/github/LinearSystemSolvers/scripts/final/')
  
  wd.folder <- '~/github/LinearSystemSolvers/scripts/final/profile1/'
  source('read-profile.R')
  setwd('~/github/LinearSystemSolvers/scripts/final/')
  
  boxplot(fix.np.prog)
  boxplot(fix.np.time)

  avg <- colMeans(fix.np.prog)
  
  old <- c(3.5, 23)
  
  png(filename = 'figures/fix-np-time.png', height = 5, width = 8, res = 300, units = 'in')
  centers <- barplot(c(avg, old), col = c(rep('purple', length(avg)),
                               rep('green', length(old))),
                     border = T, axes = F, axisnames = F,
                     xlab = 'Data Size', ylab = 'Time Elapsed (s)')
  axis(1, at = centers, labels = c(names(avg), '200 v1', '400 v1'))
  axis(2)
  
  legend('topleft', fill = c('purple', 'green'), legend = c('v2', 'v1'))
  dev.off()
}
