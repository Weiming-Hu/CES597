#!/bin/bash

# Make the pdf stuff

latex project-review
bibtex project-review
latex project-review #(repeat as needed to resolve references)
latex project-review #(repeat as needed to resolve references)

dvips project-review
ps2pdf project-review.ps
rm project-review.ps

