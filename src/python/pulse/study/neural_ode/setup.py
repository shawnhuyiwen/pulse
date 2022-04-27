from setuptools import setup, find_packages

setup(
    name='neural_ode',
    version='0.0.1',
    packages=find_packages(
        where='.'
    ),
    install_requires=[
        "einops",
        "matplotlib",
        "numpy",
        "pandas",
        "pythonic_config",
        "pytorch_forecasting",
        "torch",
        "torchcde",
        "torchdiffeq",
        "torchvision",
        "tqdm",
        "ubelt",
    ]
)
