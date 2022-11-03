#!/bin/bash

set -ex

images=(
	"ubuntu:20.04"
	"ubuntu:22.04"
	"alpine:latest"
)
ORIGIN=docker.io

podman stop registry 2>/dev/null || true
podman run --rm -d --name registry -p 5000:5000 docker.io/registry:2
until curl localhost:5000; do
	sleep 1
done

for img in ${images[@]}; do
	podman pull $ORIGIN/$img
	podman tag $ORIGIN/$img localhost:5000/$img
	podman push --tls-verify=false localhost:5000/$img

done
