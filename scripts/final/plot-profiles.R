cat("Set working directory to ~/github/LinearSystemSolvers/scripts/final\n")
setwd('~/github/LinearSystemSolvers/scripts/final/')

folders <- list.dirs(path = '.', full.names = T)[-1]

fix.np.prog <- matrix(NA, nrow = 6, ncol = 3)
fix.np.time <- matrix(NA, nrow = 6, ncol = 3)
fix.size.prog <- matrix(NA, nrow = 14, ncol = 3)
fix.size.time <- matrix(NA, nrow = 14, ncol = 3)

rownames(fix.np.prog) <- c(10, 100, 200, 400, 800, 1600)
rownames(fix.np.time) <- c(10, 100, 200, 400, 800, 1600)
rownames(fix.size.prog) <- c(1, 2, 4, 8, 16, 21, 24, 28, 32, 38, 44, 50, 55, 64)
rownames(fix.size.time) <- c(1, 2, 4, 8, 16, 21, 24, 28, 32, 38, 44, 50, 55, 64)

for (i in 1:length(folders)) {
  folder <- folders[i]
  cat("Working in the folder", folder, '\n')
  
  # Read log-fix-np-prog
  file <- paste(folder, '/log-fix-np-prog.txt', sep = '')
  if (!file.exists(file)) stop(paste(file, "not found!"))
  
  con <- file(file, "r")
  data <- readLines(con = con)
  close(con)
  
  for (j in 1:nrow(fix.np.prog)) {
    s <- gsub("Total time: (.*?)s \\(\\d*?%\\)", "\\1", data[j * 4 - 1])
    fix.np.prog[j, i] <- as.numeric(s)
  }
  
  # Read log-fix-np-time
  file <- paste(folder, '/log-fix-np-time.txt', sep = '')
  if (!file.exists(file)) stop(paste(file, "not found!"))
  
  con <- file(file, "r")
  data <- readLines(con = con)
  close(con)
  
  for (j in 1:nrow(fix.np.time)) {
    m <- gsub("real\t(.*?)m(.*?)s", "\\1", data[j * 4 - 3])
    s <- gsub("real\t(.*?)m(.*?)s", "\\2", data[j * 4 - 3])
    fix.np.time[j, i] <- as.numeric(m) * 60 + as.numeric(s)
  }
  
  # Read log-fix-size-prog
  file <- paste(folder, '/log-fix-size-prog.txt', sep = '')
  if (!file.exists(file)) stop(paste(file, "not found!"))
  
  con <- file(file, "r")
  data <- readLines(con = con)
  close(con)
  
  for (j in 1:nrow(fix.size.prog)) {
    s <- gsub("Total time: (.*?)s \\(\\d*?%\\)", "\\1", data[j * 4 - 1])
    fix.size.prog[j, i] <- as.numeric(s)
  }
  
  # Read log-fix-size-time
  file <- paste(folder, '/log-fix-size-time.txt', sep = '')
  if (!file.exists(file)) stop(paste(file, "not found!"))
  
  con <- file(file, "r")
  data <- readLines(con = con)
  close(con)
  
  for (j in 1:nrow(fix.size.time)) {
    m <- gsub("real\t(.*?)m(.*?)s", "\\1", data[j * 4 - 3])
    s <- gsub("real\t(.*?)m(.*?)s", "\\2", data[j * 4 - 3])
    fix.size.time[j, i] <- as.numeric(m) * 60 + as.numeric(s)
  }
}

fix.size.prog <- t(fix.size.prog)
fix.size.time <- t(fix.size.time)
fix.np.prog <- t(fix.np.prog)
fix.np.time <- t(fix.np.time)

boxplot(fix.size.time)
