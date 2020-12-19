# -*- coding: utf-8 -*-
"""
Created on Thu Aug 27 09:17:26 2020

@author: luisc
"""

  !pip install "tensorflow_hub>=0.6.0"
  !pip install "tensorflow>=2.0.0"

  import tensorflow as tf
  import tensorflow_hub as hub

  module_url = "https://tfhub.dev/google/nnlm-en-dim128/2"
  embed = hub.KerasLayer(module_url)
  embeddings = embed(["A long sentence.", "single-word",
                      "http://example.com"])
  print(embeddings.shape)  #(3,128)