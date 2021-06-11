# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.158.0/containers/ubuntu/.devcontainer/base.Dockerfile

# [Choice] Ubuntu version: bionic, focal
#ARG VARIANT="focal"
#FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}


FROM ubuntu:20.04

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get -y update && \
    apt-get -y install gcc-arm-none-eabi=15:9-2019-q4-0ubuntu1 &&\
    apt-get -y update && \
    apt-get -y install git \
        cmake \
        openocd \
        mc \
        git \
        gdb-multiarch &&\
    apt-get purge -y --auto-remove &&\
    apt-get -y update && \
    apt-get -y install clang

RUN useradd -ms /bin/bash vscode

COPY ./.vscode.in/ /.vscode/
#RUN mkdir -p /workspaces/SynchroLight/
#RUN ln -s ./.vscode.in/ /workspaces/SynchroLight/.vscode

#USER vscode

# [Optional] Uncomment this section to install additional OS packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>


