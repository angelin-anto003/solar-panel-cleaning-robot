
import streamlit as st
import cv2
import numpy as np
from PIL import Image

st.set_page_config(
    page_title="Solar Panel Fault Detection",
    page_icon="☀️",
    layout="wide"
)

st.title("☀️ Solar Panel Fault Detection")
st.write(
    "Upload a solar panel image to begin inspecting "
    "its visible surface condition."
)

uploaded_file = st.file_uploader(
    "Choose a solar panel image",
    type=["jpg", "jpeg", "png"]
)

if uploaded_file is not None:
    image = Image.open(uploaded_file).convert("RGB")
    image_array = np.array(image)

    st.subheader("Uploaded Image")
    st.image(image_array, use_container_width=True)

    st.subheader("Basic Image Processing")

    gray_image = cv2.cvtColor(
        image_array, cv2.COLOR_RGB2GRAY
    )

    edges = cv2.Canny(gray_image, 100, 200)

    col1, col2 = st.columns(2)

    with col1:
        st.write("Grayscale Image")
        st.image(gray_image, use_container_width=True)

    with col2:
        st.write("Detected Edges")
        st.image(edges, use_container_width=True)

    st.info(
        "This is an initial image-processing prototype. "
        "It does not yet classify dust, dirt, or bird droppings."
    )
else:
    st.caption(
        "Upload an image to view the image-processing results."
    )
