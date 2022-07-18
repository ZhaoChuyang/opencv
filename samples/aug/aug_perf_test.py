import sys
from typing import List, Union
import os
import numpy as np
import cv2
import matplotlib.pyplot as plt
import pickle
import time
import torchvision.transforms as T
import PIL
import torch


_available_datasets = {
    "cifar-10":  "/Users/chuyang/Downloads/cifar-10-batches-py",
    "imagenet-full": "",
    "imagenet-320": "/Users/chuyang/Downloads/imagenette2-320/train/n01440764",
}


def load_dataset(dataset: str, return_tensor: bool = False) -> List[Union[np.array, torch.Tensor]]:
    """
    Args:
    - dataset: (str) the name of the dataset
    - return_tensor: (bool) return tensor or not. return np.ndarray if set to False, otherwise return torch.tensor

    Returns:
        list of loaded image files
    """
    assert dataset in _available_datasets

    if dataset == 'cifar-10':
        path = _available_datasets[dataset]
        data_files = ["data_batch_1", "data_batch_2", "data_batch_3", "data_batch_4", "data_batch_5"]
        # use data_batch_1 by default
        with open(os.path.join(path, data_files[0]), 'rb') as fb:
            raw_data = pickle.load(fb, encoding='bytes')
        data = raw_data[b'data']

        all_images = []
        data_wrapper = lambda x: torch.tensor(x).permute(2, 0, 1) if return_tensor else x
        for i in range(len(data)):
            img = data[i, :].reshape((32, 32, 3), order='F')
            all_images.append(data_wrapper(img))

        return all_images

    if dataset in ["imagenet-full", "imagenet-320"]:
        path = _available_datasets[dataset]
        all_images = []
        data_wrapper = lambda x: torch.tensor(x).permute(2, 0, 1) if return_tensor else x
        for filename in os.listdir(path):
            if filename.split('.')[-1] == 'JPEG':
                img = cv2.imread(os.path.join(path, filename))
                all_images.append(data_wrapper(img))
        return all_images

    raise NotImplementedError("dataset {} is not recognized!".format(dataset))


def runtime(func):
    def func_wrapper(*args, **kwargs):
        start = time.time()
        func(*args, **kwargs)
        end = time.time()
        return end - start
    return func_wrapper


@runtime
def ocv_aug_func(dataset, transform, arguments):
    for img in dataset:
        transform(img, *arguments)


@runtime
def ocv_aug_class(dataset, transform):
    for img in dataset:
        transform.call(img)


@runtime
def pytorch_aug_class(dataset, transform):
    for img in dataset:
        transform(img)


# def test_ocv_random_crop_func():
#     cifar_10 = load_dataset("cifar-10")
#     transform = cv2.randomCrop
#     arguments = [np.array([20, 20])]
#     total_time = ocv_aug_func(cifar_10, transform, arguments)
#     print("test_ocv_random_crop_func", total_time)
#     del cifar_10


def test_ocv_random_crop_func():
    dataset = load_dataset("imagenet-320")
    transform = cv2.randomCrop
    # transform = cv2.randomCropV1
    arguments = [np.array([200, 200])]
    total_time = ocv_aug_func(dataset, transform, arguments)
    print("test_ocv_random_crop_func", total_time)
    del dataset


# def test_pytorch_random_crop():
#     cifar_10 = load_dataset("cifar-10", return_tensor=True)
#     transform = T.RandomCrop((20, 20))
#     total_time = pytorch_aug_class(cifar_10, transform)
#     print("test_pytorch_random_crop", total_time)
#     del cifar_10


def test_pytorch_random_crop():
    dataset = load_dataset("imagenet-320", return_tensor=True)
    transform = T.RandomCrop((200, 200))
    total_time = pytorch_aug_class(dataset, transform)
    print("test_pytorch_random_crop", total_time)
    del dataset


def main():
    test_ocv_random_crop_func()
    test_pytorch_random_crop()


if __name__ == '__main__':
    print(sys.argv)
    main()
#
# crop_size = np.array([20, 20])
# randomCrop = cv2.RandomCrop(crop_size)
# randomFlip = cv2.RandomFlip()
# transforms = cv2.Compose([randomCrop, randomFlip])
#
#
# start = time.time()
# for img in all_images:
#     # img = cv2.randomCrop(img, crop_size)
#     # img = randomCrop.call(img)
#     img = transforms.call(img)
# end = time.time()
#
# print("Run Time (OpenCV) : %f" % (end-start))
#
#
# randomCrop = T.RandomCrop(size=(20, 20))
# randomFlip = T.RandomHorizontalFlip()
# transforms = T.Compose([randomCrop, randomFlip])
# all_images = []
# for i in range(len(data)):
#     img = data[i, :].reshape((32, 32, 3), order='F')
#     img = PIL.Image.fromarray(img)
#     # img = torch.tensor(img).permute(2, 0, 1)
#     all_images.append(img)
#
# start = time.time()
# for img in all_images:
#     # img = randomCrop(img)
#     img = transforms(img)
# end = time.time()
#
# print("Run Time (PyTorch) : %f" % (end-start))
