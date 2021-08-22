[![Build][ot-gh-action-build-svg]][ot-gh-action-build]

[ot-gh-action-build]: https://github.com/openthread/ot-cc13x2-cc26x2/actions?query=workflow%3ABuild+branch%3Amain+event%3Apush
[ot-gh-action-build-svg]: https://github.com/openthread/ot-cc13x2-cc26x2/workflows/Build/badge.svg?branch=main&event=push

---

# OpenThread on CC13x2/CC26x2 Examples

This repo contains example platform drivers for the [Texas Instruments CC1352R1][cc1352r1] and [Texas Instruments CC2652R1][cc2652r1].

[cc1352r1]: http://www.ti.com/tool/launchxl-cc26x2r1
[cc2652r1]: http://www.ti.com/tool/launchxl-cc26x2r1

The example platform drivers are intended to present the minimal code necessary to support OpenThread. As a result, the example platform drivers do not necessarily highlight the platform's full capabilities. Consult the [SimpleLink CC26X2R1 SDK][cc26x2r1-sdk] for more development option. The platform drivers were built for the [CC1352R1 LAUNCHXL][cc1352r1-launchxl], usage on other boards with a cc1352r1 may require changes to the peripheral drivers.

[cc1352r1-launchxl]: http://www.ti.com/tool/launchxl-cc26x2r1
[cc26x2r1-sdk]: http://www.ti.com/tool/simplelink-cc26x2-sdk

To learn more about building and running the examples please check:

- [OpenThread on CC1352R1 examples][cc1352r1-page]
- [OpenThread on CC2652R1 examples][cc2652r1-page]

[cc1352r1-page]: ./src/cc1352/README.md
[cc2652r1-page]: ./src/cc2652/README.md

# Contributing

We would love for you to contribute to OpenThread and help make it even better than it is today! See our [Contributing Guidelines](https://github.com/openthread/openthread/blob/main/CONTRIBUTING.md) for more information.

Contributors are required to abide by our [Code of Conduct](https://github.com/openthread/openthread/blob/main/CODE_OF_CONDUCT.md) and [Coding Conventions and Style Guide](https://github.com/openthread/openthread/blob/main/STYLE_GUIDE.md).

# License

OpenThread is released under the [BSD 3-Clause license](https://github.com/openthread/ot-cc13x2-cc26x2/blob/main/LICENSE). See the [`LICENSE`](https://github.com/openthread/ot-cc13x2-cc26x2/blob/main/LICENSE) file for more information.

Please only use the OpenThread name and marks when accurately referencing this software distribution. Do not use the marks in a way that suggests you are endorsed by or otherwise affiliated with Nest, Google, or The Thread Group.

# Need help?

There are numerous avenues for OpenThread support:

- Bugs and feature requests — [submit to the Issue Tracker](https://github.com/openthread/openthread/issues)
- Stack Overflow — [post questions using the `openthread` tag](http://stackoverflow.com/questions/tagged/openthread)
- Google Groups — [discussion and announcements at openthread-users](https://groups.google.com/forum/#!forum/openthread-users)

The openthread-users Google Group is the recommended place for users to discuss OpenThread and interact directly with the OpenThread team.
