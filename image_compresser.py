#! /usr/bin/env python3
# image_compresser.py
# Drew Bies
# tutorial from Rick Wierenga

# description: compresses an image file using the k-means algorithm. 
# --file must be png--

import os
import sys
import time

from PIL import Image
import numpy as np


# Load image from path. Return a numpy array
def load_image(path):
    image = Image.open(path)
    return np.asarray(image) / 255

# Choose K points from X at random
def initialize_K_centroids(X, K):
    m = len(X)
    return X[np.random.choice(m, K, replace=False), :]

# takes the image matrix and the centroids and returns the closest ones
def find_closest_centroids(X, centroids):
    m = len(X)
    c = np.zeros(m)
    
    for i in range(m):
        # Find distances
        distances = np.linalg.norm(X[i] - centroids, axis=1)

        # Assign closest cluster to c[i]
        c[i] = np.argmin(distances)
    return c


def compute_means(X, idx, K):
    _, n = X.shape
    centroids = np.zeros((K, n))
    for k in range(K):
        examples = X[np.where(idx == k)]
        mean = [np.mean(column) for column in examples.T]
        centroids[k] = mean
    return centroids


def find_k_means(X, K, max_iters=10):
    centroids = initialize_K_centroids(X, K)
    previous_centroids = centroids
    for _ in range(max_iters):
        idx = find_closest_centroids(X, centroids)
        centroids = compute_means(X, idx, K)
        # check if the centroids are still moving
        if (previous_centroids==centroids).all():
            return centroids
        else:
            previous_centroids = centroids
    return centroids, idx


def main():
    # check if the image exists
    try:
        image_path = sys.argv[1]
        assert os.path.isfile(image_path)
    except (IndexError, AssertionError):
        sys.exit('Please choose a valid image (PNG) path')

    # get size of the image file
    path_size = os.path.getsize(image_path)
    print('Size of image file: {} bytes'.format(path_size))

    # Load the image
    image = load_image(image_path)
    w, h, d = image.shape
    #print('Image found with width: {}, height: {}, depth: {}'.format(w, h, d))

    # Get the feature matrix X
    X = image.reshape((w * h, d))
    # the number of colors in the image
    K = 60

    # Get colors using the k-means algorithm
    print('Running K-means algorithm')
    start_time = time.time()
    colors, _ = find_k_means(X, K, max_iters=20)
    # get the time for the k-means process
    process_time = round(time.time() - start_time, 2)
    

    # Indexes for color for each pixel
    idx = find_closest_centroids(X, colors)

    # Reconstruct the image
    idx = np.array(idx, dtype=np.uint8)
    X_reconstructed = np.array(colors[idx, :] * 255, dtype=np.uint8).reshape((w, h, d))
    compressed_image = Image.fromarray(X_reconstructed)

    # Save reconstructed image to disk
    compressed_image.save('out.png')
    # get the size of the new image file
    new_path_size = os.path.getsize('out.png')

    # print stats
    print('Size of compressed image file: {} bytes'.format(new_path_size))
    print('Image was compressed by {}%'.format(round(path_size / new_path_size * 10, 2)))
    print('k-means algorithm process time: {} seconds'.format(process_time))


if __name__ == '__main__':
    main()