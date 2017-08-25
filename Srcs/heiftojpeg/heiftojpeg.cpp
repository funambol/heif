#include "hevcimagefilereader.hpp"
#include <iostream>

using namespace std;

int main()
{
    printf("A\n");
    HevcImageFileReader reader;
    ImageFileReaderInterface::DataVector data;
    ImageFileReaderInterface::IdVector itemIds;

    printf("B\n");
    reader.initialize("test_002.heic");
    printf("C\n");
    const auto& properties = reader.getFileProperties();
    printf("D\n");

    // Verify that the file has one or several images in the MetaBox
    if (not (properties.fileFeature.hasFeature(ImageFileReaderInterface::FileFeature::HasSingleImage) ||
        properties.fileFeature.hasFeature(ImageFileReaderInterface::FileFeature::HasImageCollection)))
    {
        printf("Input has no image\n");
        return 1;
    }

    printf("E\n");

    // Find the item ID of the first master image
    const uint32_t contextId = properties.rootLevelMetaBoxProperties.contextId;
    reader.getItemListByType(contextId, "master", itemIds);
    printf("F\n");
    const uint32_t masterId = itemIds.at(0);

    const auto& metaBoxFeatures = properties.rootLevelMetaBoxProperties.metaBoxFeature; // For convenience
    if (metaBoxFeatures.hasFeature(ImageFileReaderInterface::MetaBoxFeature::HasThumbnails))
    {
        // Thumbnail references ('thmb') are from the thumbnail image to the master image
        reader.getReferencedToItemListByType(contextId, masterId, "thmb", itemIds);
        const uint32_t thumbnailId = itemIds.at(0);

        reader.getItemDataWithDecoderParameters(contextId, thumbnailId, data);
        // ...decode data and display the image, show master image later
    }
    else
    {
        // There was no thumbnail, show just the master image
        reader.getItemDataWithDecoderParameters(contextId, masterId, data);
        // ...decode and display...
    }

    return 0;
}
