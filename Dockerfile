FROM debian:12

RUN apt-get update && \
    apt-get install -y curl && \
    apt-get clean

RUN curl -LO https://packages.microsoft.com/config/debian/12/packages-microsoft-prod.deb && \
    dpkg -i packages-microsoft-prod.deb && \
    rm packages-microsoft-prod.deb

RUN apt-get update && \
    apt-get install -y bmake build-essential clang libssl-dev \
      mono-devel dotnet-sdk-6.0 && \
    apt-get clean

WORKDIR /work
COPY . /work/
RUN bmake
CMD bmake run
