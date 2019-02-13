setwd('~/github/LinearSystemSolvers/scripts/report2/')

ori <- c(0.12592, 0.67309, 2.04280, 4.67890, 8.51510, 15.34400, 22.52300, 35.69900)
imp <- c(0.104998, 0.576980, 1.498300, 4.169400, 6.211200, 12.121000, 17.509000, 23.169000)
size <- c(50, 100, 150, 200, 250, 300, 350, 400)

speedup <- ori/imp

png('serial-improve.png', width = 9, height = 6, res = 400, units = 'in')

par(mar = c(5, 5, 1, 5))
plot(size, ori, type = 'b', pch = 1, lwd = 1.5,
     xlab = 'Data Size', ylab = 'Time (s)')
lines(size, imp, type = 'b', pch = 2, lwd = 1.5)

par(new = T)
plot(speedup, type = 'l', lty = 'dashed', yaxt = 'n', xaxt = 'n',
     ylab = '', xlab = '', ylim = c(0.8, 1.8), col = 'red', lwd = 1.5)
axis(4, col = 'red', col.axis = 'red')
axis(4, col = 'red', col.axis = 'red')
mtext(side = 4, line = 3, 'Speed Up', col = 'red')

legend('topleft', lty = c('solid', 'solid', 'dashed'),
       pch = c(1, 2, -1), col = c('black', 'black', 'red'),
       legend = c('Original', 'Modified', 'Speedup'))

dev.off()