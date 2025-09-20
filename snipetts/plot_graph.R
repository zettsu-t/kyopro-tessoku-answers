library(igraph)

in_filename <- "graph.txt"
out_filename <- "graph.png"

n_cols <- NCOL(readr::read_delim(in_filename, delim = " "))
print(n_cols)

df <- if (n_cols >= 3) {
  readr::read_delim(in_filename, col_names=c("from", "to", "weight"), delim = " ")
} else {
  readr::read_delim(in_filename, col_names=c("from", "to"), delim = " ")
}

g <- igraph::graph_from_data_frame(df, directed = FALSE)

E(g)$label <- if (n_cols >= 3) {
  E(g)$weight
} else {
  "_"
}

plot_impl <- function() {
  plot(g, layout=layout.circle, vertex.color="azure", vertex.label.color="black", vertex.label.cex=2, edge.label.color="darkorchid", edge.label.cex=1.3)
}

plot_impl()
png(out_filename, width=800, height=800)
plot_impl()
dev.off()
