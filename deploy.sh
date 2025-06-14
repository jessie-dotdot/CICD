#!/bin/bash

  scp src/C3_SimpleBashUtils/cat/s21_cat gitlab-runner@10.10.0.1:~/
  scp src/C3_SimpleBashUtils/grep/s21_grep gitlab-runner@10.10.0.1:~/


  ssh gitlab-runner@10.10.0.1 "echo "Done" | sudo -S mv s21_cat s21_grep /usr/local/bin/"

echo "Deploy success1"
