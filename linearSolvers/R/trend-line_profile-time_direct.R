# Read input csv file and plot the trend line of profile time for the direct solver
library(reshape2)

table <- read.csv('../output-profile-optimization/profile_time_direct.csv', header = T)
colnames(table) <- c("50", "100", "250", "500", "750", "1000", "1100", "1200", "1300")

data <- melt(table, measure.vars = colnames(table))
colnames(data) <- c('x', 'y')
data$y <- as.numeric(as.character(data$y))
data$x <- as.numeric(as.character(data$x))

# Fit models
new.x <- seq(50, 1500, length.out = 1000)

model.3 <- lm(y ~ I(x^3), data = data)
model.3.new.y <- predict(model.3, data.frame(x = new.x))

model.2 <- lm(y ~ I(x^2), data = data)
model.2.new.y <- predict(model.2, data.frame(x = new.x))

png('profile_time_direct.png', width = 8, height = 6, units = 'in', res = 500)
plot(data, type = 'n', xlab = 'Size', ylab = 'Time (s)',
     xlim = range(new.x), ylim = range(model.3.new.y))
lines(new.x, model.3.new.y, col = 'red', lty = 'dashed', lwd = 2)
lines(new.x, model.2.new.y, col = 'purple', lty = 'dotdash', lwd = 2)
points(data, pch = 16, cex = .8, xlab = 'Size', ylab = 'Time (s)',
     xlim = range(new.x), ylim = range(model.3.new.y))
legend('topleft', legend = c('O(n^2)', 'O(n^3)'), col = c('purple', 'red'),
       lty = c('dotdash', 'dashed'), lwd = 2)
dev.off()
