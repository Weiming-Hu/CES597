# Read input csv file and plot the trend line of profile time for the direct solver
library(reshape2)

table.random <- read.csv('../output-profile-optimization/profile_time_gauss_random.csv', header = T)
table.all1s <- read.csv('../output-profile-optimization/profile_time_gauss_all1s.csv', header = T)

colnames(table.random) <- c("50", "100", "250", "500", "750", "1000", "1100", "1200", "1300")
colnames(table.all1s) <- c("50", "100", "250", "500", "750", "1000", "1100", "1200", "1300")

data.random <- melt(table.random, measure.vars = colnames(table.random))
data.all1s <- melt(table.all1s, measure.vars = colnames(table.all1s))

colnames(data.random) <- c('x', 'y')
colnames(data.all1s) <- c('x', 'y')

data.random$y <- as.numeric(as.character(data.random$y))
data.random$x <- as.numeric(as.character(data.random$x))
data.all1s$y <- as.numeric(as.character(data.all1s$y))
data.all1s$x <- as.numeric(as.character(data.all1s$x))

# Fit models
new.x <- seq(50, 1500, length.out = 1000)

model.3.random <- lm(y ~ I(x^3), data = data.random)
model.3.random.new.y <- predict(model.3.random, data.frame(x = new.x))

model.3.all1s <- lm(y ~ I(x^3), data = data.all1s)
model.3.all1s.new.y <- predict(model.3.all1s, data.frame(x = new.x))

model.2.random <- lm(y ~ I(x^2), data = data.random)
model.2.random.new.y <- predict(model.2.random, data.frame(x = new.x))

model.2.all1s <- lm(y ~ I(x^2), data = data.all1s)
model.2.all1s.new.y <- predict(model.2.all1s, data.frame(x = new.x))

png('profile_time_gauss.png', width = 8, height = 6, units = 'in', res = 500)
plot(data.random, type = 'n', xlab = 'Size', ylab = 'Time (s)',
     xlim = range(new.x), ylim = range(model.3.random.new.y))

lines(new.x, model.3.random.new.y, col = 'red', lty = 'dotted', lwd = 2)
lines(new.x, model.2.random.new.y, col = 'purple', lty = 'dotdash', lwd = 2)

# lines(new.x, model.3.all1s.new.y, col = 'red', lty = 'solid', lwd = 2)
# lines(new.x, model.2.all1s.new.y, col = 'red', lty = 'dotdash', lwd = 2)

points(data.random, pch = 16, cex = 1, col = 'purple')
points(data.all1s, pch = 3, col = 'red', lwd = .5)

legend('topleft', legend = c('O(n^2)', 'O(n^3)', 'Random initialization', 'All 1 initialization'),
       col = c('purple', 'red', 'purple', 'red'), lty = c('dotdash', 'dashed', NA, NA),
       pch = c(NA, NA, 16, 3), lwd = 2)
dev.off()
