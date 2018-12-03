if (T) {
  space <- c(7049079, 7132747, 7377451, 8346891, 12206315, 27398322) / 1000
  size <- c(10, 100, 200, 400, 800, 1600)
  file.size <- c(8, 88, 324, 1300, 5000, 20000)
  
  ylim <- range(file.size, space)
  
  png('figures/profile-space.png', width = 9, height = 7, units = 'in', res = 400)
  
  plot(size, space, type = 'b', xaxt = 'n', pch = 1, ylim = ylim,
       xlab = 'Data Size', ylab = 'Heap Used (kb)', lwd = 2)
  lines(size, file.size, col = 'red', lty = 'dashed', pch = 2, type = 'b', lwd = 2)
  axis(1, at = size, labels = size)
  legend('topleft', col = c('black', 'red'), pch = c(1, 2), lwd = 2,
         lty = c('solid', 'dashed'), legend = c('memory', 'file size'))
  
  points(100, 16960, pch = 3, lwd = 2, cex = 1.5, col = 'green')
  points(100, 16960, pch = 1, lwd = 2, cex = 1.5, col = 'green')
  text(labels = 'Parallel \nJacobi version 1', x = 100, y = 16960 + 1600)
  
  points(100, 1505, pch = 3, lwd = 2, cex = 1.5, col = 'green')
  points(100, 1505, pch = 1, lwd = 2, cex = 1.5, col = 'green')
  text(labels = 'Serial Jacobi', x = 100, y = 1505 + 1000)
  dev.off()
}
