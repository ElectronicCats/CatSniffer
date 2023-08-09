from setuptools import setup


setup(
    name="cc2538-bsl",
    description="Script to communicate with Texas Instruments CC13xx/CC2538/CC26xx Serial Boot Loader .",
    long_description=open("README.md", encoding="utf-8").read(),
    keywords="cc2538, cc1310, cc13xx, bootloader, cc26xx, cc2650, cc2640",
    url="https://github.com/JelmerT/cc2538-bsl",
    author="Jelmer Tiete",
    author_email="jelmer@tiete.be",
    license="BSD-3-Clause",
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Environment :: Console",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: BSD License",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS",
        "Programming Language :: Python :: 3",
        "Topic :: Scientific/Engineering",
    ],
    platforms="posix",
    python_requires=">=3.4",
    setup_requires=["setuptools_scm"],
    use_scm_version=lambda: {
        "version_scheme": "post-release",
        "local_scheme": "node-and-date",
        },
    install_requires=["pip>=10", "setuptools", "wheel", "pyserial"],
    extras_require={
        '': ["intelhex"],
        'intelhex': ["python-magic"]
    },
    scripts=["cc2538-bsl.py"],
)
