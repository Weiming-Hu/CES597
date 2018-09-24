# Read csv file and plot the boxplot
table <- read.csv('../output-profile-optimization/time_direct.csv', header = T,
                  col.names = c("O0", "O1", "O2", "O3", "Ofast"))
png('time_direct.png', width = 5, height = 6, units = 'in', res = 500)
boxplot(table, xlab = 'Optimization Flag', ylab = 'Time (seconds)',
        outline = F)
dev.off()
