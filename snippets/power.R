library(gmp)
library(purrr)

nums <- purrr::map_dbl(1:128, ~ gmp::asNumeric(gmp::pow.bigz(2, .x) %% 1000000007))

purrr::reduce(.x=2:32, .init=list(gmp::as.bigz(2)), .f=function(acc, i) {
   v <- acc[[NROW(acc)]]
   v <- v * v
   v <- v %% 1000000007
   c(acc, list(v))
}) %>%
  purrr::map_dbl(gmp::asNumeric)
