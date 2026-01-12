library(igraph)

in_filename <- "graph.txt"

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

plot_impl <- function(in_layout, out_filename) {
  plot(g, layout=in_layout, vertex.color="azure", vertex.label.color="black", vertex.label.cex=2, edge.label.color="darkorchid", edge.label.cex=1.3)
}

plot_all <- function(in_layout, out_filename) {
  png(out_filename, width=800, height=800)
  plot_impl(in_layout=in_layout, out_filename=out_filename)
  dev.off()
  plot_impl(in_layout=in_layout, out_filename=out_filename)
}

plot_all(in_layout=igraph::layout_nicely, out_filename="graph.png")
plot_all(in_layout=igraph::layout_in_circle, out_filename="graph_circle.png")
tree_layout <- igraph::layout_as_tree(g, root=1)
plot_all(in_layout=tree_layout, out_filename="graph_tree.png")
