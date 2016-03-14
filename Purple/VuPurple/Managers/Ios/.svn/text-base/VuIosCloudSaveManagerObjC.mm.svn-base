//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
//
//  Objective-C support for Ios cloud data manager.
//
//*****************************************************************************

#include "VuIosCloudSaveManagerObjC.h"


// Internal data

struct IOSiCloudData
{
	bool m_IsAvailable;				//!< Is iCloud available at all?
	char m_Name[256];				//!< Managed iCloud file name
    
	bool m_SupportConflictResolution; //!< Indicates whether to support conflict detection and resolution
	
	bool m_RetryRead;               //!< Did last read fail? If so, retry
	float m_RetryReadTimer;
	int m_RetryReadCounter;         //!< Counter/timer for read retrying
	
	void* m_ToWriteData;            //!< Data to write to iCloud file
	int m_ToWriteDataSize;          //!< Size of the data to write to iCloud
	bool m_ReadyForWriting;         //!< Indicates whether we're ready to write (either determined that the file in iCloud doesn't exist, or the file exists but hasn't been read yet)
	bool m_WriteInProgress;         //!< Indicates whether write operation is in progress (which prevents other reads)
	bool m_ScheduledWriting;        //!< Indicates whether write operation is scheduled to be done as soon as possible
	bool m_WriteSucceeded;          //!< Indicates whether last write attempt succeeded or failed (TODO: to be used for write retrying)
	
	IOSiCloudData() :
	m_IsAvailable(false),
	m_SupportConflictResolution(true),
	m_RetryRead(false),
	m_RetryReadTimer(0.0f),
	m_RetryReadCounter(0),
	m_ToWriteData(NULL),
	m_ToWriteDataSize(0),
	m_ReadyForWriting(false),
	m_WriteInProgress(false),
	m_ScheduledWriting(false),
	m_WriteSucceeded(false)
	{
		m_Name[0] = 0;
	}
};

// Query observer interface

@interface IOSiCloudQueryObserver : NSObject
- (void)queryDidFinishGathering: (NSNotification*) notification;
@end

// iCloud synchronized document interface

@interface IOSiCloudDoc : UIDocument
- (void)documentStateChanged: (NSNotification*) notification;
- (void)resolveConflicts;
@end

// Static variables

static VuIosCloudSaveManagerObjC::CallbackIF *g_callback = VUNULL;
static IOSiCloudData g_data;
static NSMetadataQuery* g_query = nil;
static IOSiCloudQueryObserver* g_queryObserver = nil;
static IOSiCloudDoc* g_doc = nil;
static NSFileCoordinator* g_coordinator = nil;


// Misc

void IOSiCloudStartReading()
{
	VUPRINTF("iCloud: Starting read\n");
	
	[g_doc openWithCompletionHandler:^(BOOL success)
	 {
		 VUPRINTF("iCloud: Read %s\n", success ? "succeeded" : "failed");
	 }];
}

void IOSiCloudCreateDoc(NSURL* url)
{
	VUPRINTF("iCloud: Creating document\n");
	
	g_doc = [[IOSiCloudDoc alloc] initWithFileURL:url];
    
	VUPRINTF("iCloud: Adding document state changed observer\n");
	
	[[NSNotificationCenter defaultCenter]
	 addObserver: g_doc
	 selector: @selector(documentStateChanged:)
	 name: UIDocumentStateChangedNotification
	 object: g_doc];
}

// IOSiCloudQueryObserver implementation

@implementation IOSiCloudQueryObserver

- (void)queryDidFinishGathering: (NSNotification*) notification
{
    VUPRINTF("iCloud: queryDidFinishGathering\n");
	
	// Stop query
    
	VUPRINTF("iCloud: Stopping query\n");
    
	[g_query disableUpdates];
	[g_query stopQuery];
    
	// Process query result
    
	VUPRINTF("iCloud: Query result contains %d entries\n", [g_query resultCount]);
    
	if ([g_query resultCount] == 1)
	{
		NSMetadataItem *item = [g_query resultAtIndex:0];
		NSURL *url = [item valueForAttribute:NSMetadataItemURLKey];
		IOSiCloudCreateDoc(url);
		IOSiCloudStartReading();
	}
	else
	{
		VUPRINTF("iCloud: Ready for writing\n");
		g_data.m_ReadyForWriting = true;
	}
}

- (void) dealloc
{
	// Remove query observer
    
	VUPRINTF("iCloud: Removing query observer\n");
    
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:NSMetadataQueryDidFinishGatheringNotification
												  object:g_query];
// ARC enabled
//	[super dealloc];
}

@end

// IOSiCloudDoc implementation

@implementation IOSiCloudDoc

- (void)resolveConflicts
{
	VUPRINTF("iCloud: Started resolving conflict\n");
	
	// We've got conflict! - first get all conflicting versions
	
	NSArray* versions = [NSFileVersion unresolvedConflictVersionsOfItemAtURL: [self fileURL]];
	VUPRINTF("iCloud: Found %d conflicting versions\n", [versions count]);
	
	// Merge all conflicting versions (one by one)
	
	if (!g_coordinator)
	{
		g_coordinator = [[NSFileCoordinator alloc] initWithFilePresenter: self];
		VUPRINTF("iCloud: File coordinator created\n");
	}
    
	for (NSFileVersion* version in versions)
	{
		VUPRINTF("iCloud: Started coordinated read\n");
		
		NSError* readError;
		[g_coordinator coordinateReadingItemAtURL: version.URL
										  options:0
											error:&readError
									   byAccessor:^(NSURL* url)
		 {
			 VUPRINTF("iCloud: Coordinated read - getting the data\n");
			 
			 NSData* nsdata = [NSData dataWithContentsOfURL: url];
			 
			 VUPRINTF("iCloud: Calling merge(conflict) callback\n");
			 
			 if ( g_callback && g_callback->onCloudMerge([nsdata bytes], [nsdata length]) )
			 {
				 VUPRINTF("iCloud: Setting conflicting document version to resolved\n");
				 version.resolved = YES;
			 }
			 else
			 {
				 VUPRINTF("iCloud: Conflicting document version stays as unresolved\n");
			 }
		 }
		 ];
	}
	
	VUPRINTF("iCloud: Resolving conflict handling done (failed or succeeded)\n");
}

- (void)documentStateChanged: (NSNotification*) notification
{
	// Get document state
	
	UIDocumentState state = [self documentState];
	VUPRINTF("iCloud: Document state changed to %d\n", (int) state);
	
	// Check for conflict
    
	if ((state & UIDocumentStateInConflict) == 0)
		return;
	
	if (!g_data.m_SupportConflictResolution)
	{
		VUPRINTF("iCloud: Conflict detected - ignoring since s3eIOSiCloudStart wasn't invoked with conflict resolution support enabled\n");
		return;
	}
	
	VUPRINTF("iCloud: Conflict detected - scheduling asynchronous conflict resolution\n");
    
	// Schedule asynchronous conflict resolution
	
	dispatch_queue_t backgroundQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	dispatch_async(backgroundQueue, ^
				   {
					   [self resolveConflicts];
				   });
}

- (void)dealloc
{
	VUPRINTF("iCloud: Removing document state changed observer\n");
	
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:UIDocumentStateChangedNotification
												  object:self];
// ARC enabled
//	[super dealloc];
}

- (BOOL)loadFromContents:(id)contents ofType:(NSString *)typeName error:(NSError **)outError
{
	VUPRINTF("iCloud: loadFromContents (received NSData)\n");
	
	NSData* nsdata = (NSData*) contents;
	
	VUPRINTF("iCloud: Calling merge(read) callback (%d bytes)\n", [nsdata length]);
	
	if ( g_callback && g_callback->onCloudMerge([nsdata bytes], [nsdata length]) )
	{
		g_data.m_ReadyForWriting = true;
		VUPRINTF("iCloud: Ready for writing\n");
	}
	else
	{
		g_data.m_RetryRead = true;
		g_data.m_RetryReadTimer = 0.0f;
		g_data.m_RetryReadCounter = 0;
		VUPRINTF("iCloud: Read retry scheduled\n");
	}
	
	return YES;
}

- (id)contentsForType:(NSString *)typeName error:(NSError **)outError
{
	VUPRINTF("iCloud: contentsForType (saving NSData)\n");
	
	// Nothing to write?
	
	if (!g_data.m_ToWriteData)
	{
		VUPRINTF("iCloud: Nothing to write\n");
		return nil;
	}
	
	// Convert our data to NSData
	
	VUPRINTF("iCloud: Converting the data to NSData (%d bytes)\n", g_data.m_ToWriteDataSize);
	NSData* nsdata = [NSData dataWithBytes: g_data.m_ToWriteData length: g_data.m_ToWriteDataSize];
	VUPRINTF("iCloud: Data converted to NSData\n");
    
	return nsdata;
}

@end


//*****************************************************************************
void VuIosCloudSaveManagerObjC::setCallback(CallbackIF *pCB)
{
	g_callback = pCB;
}

//*****************************************************************************
bool VuIosCloudSaveManagerObjC::start(const char *fileName, bool supportConflictResolution)
{
	// Verify we're runnig iOS 5.0 or later (otherwise we'd crash while trying to get NSURL for ubiquity container identifier)

	NSArray* versionCompatibility = [[UIDevice currentDevice].systemVersion componentsSeparatedByString:@"."];

	if ([versionCompatibility count] >= 2)
	{
		VUPRINTF("iCloud: Running iOS %d.%d.\n", [[versionCompatibility objectAtIndex:0] intValue], [[versionCompatibility objectAtIndex:1] intValue]);
	}

	if ([[versionCompatibility objectAtIndex:0] intValue] < 5)
	{
		g_data.m_IsAvailable = false;
		VUPRINTF("No iCloud support on iOS earlier than 5.0\n");
		return false;
	}
	
	// Verify iCloud is available on device
	
	NSURL* ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier: nil];
	if (!ubiq)
	{
		g_data.m_IsAvailable = false;
		VUPRINTF("iCloud unavailable - failed to get NSURL for ubiquity container identifier\n");
		return false;
	}
	
	g_data.m_IsAvailable = true;
	VUPRINTF("iCloud access at '%s'\n", [[ubiq absoluteString] UTF8String]);
	
	// Store input data

	g_data.m_SupportConflictResolution = supportConflictResolution;
	strcpy(g_data.m_Name, fileName);
	
	// Create query observer
	
	g_queryObserver = [[IOSiCloudQueryObserver alloc] init];
	
	// Create query
	
	VUPRINTF("iCloud: Creating query\n");
	
	g_query = [[NSMetadataQuery alloc] init];
	[g_query setSearchScopes:[NSArray arrayWithObject: NSMetadataQueryUbiquitousDocumentsScope]];
	
	VUPRINTF("iCloud: Creating query predicate\n");
	NSString* fileNameString = [[NSString alloc] initWithUTF8String: fileName];
	NSPredicate* pred = [NSPredicate predicateWithFormat: @"%K == %@", NSMetadataItemFSNameKey, fileNameString];
// ARC enabled
//	[fileNameString release];
	[g_query setPredicate: pred];
	[[NSNotificationCenter defaultCenter]
	 addObserver: g_queryObserver
	 selector: @selector(queryDidFinishGathering:)
	 name: NSMetadataQueryDidFinishGatheringNotification
	 object: g_query];
	
	// Start query
	
	VUPRINTF("iCloud: Starting query\n");
	
	[g_query startQuery];
    
	VUPRINTF("iCloud: Query started\n");
	
	return true;
}

//*****************************************************************************
void VuIosCloudSaveManagerObjC::stop()
{
	if (!g_data.m_IsAvailable)
		return;
    
	VUPRINTF("iCloud: Destroying query observer\n");
	
	if (g_queryObserver)
	{
// ARC enabled
//		[g_queryObserver release];
		g_queryObserver = nil;
	}
	
	VUPRINTF("iCloud: Destroying query\n");
	
	if (g_query)
	{
// ARC enabled
//		[g_query release];
		g_query = nil;
	}
    
	VUPRINTF("iCloud: Destroying file coordinator\n");
    
	if (g_coordinator)
	{
		[g_coordinator cancel];
// ARC enabled
//		[g_coordinator release];
		g_coordinator = nil;
	}
    
	VUPRINTF("iCloud: Destroying document\n");
    
	if (g_doc)
	{
// ARC enabled
//		[g_doc release];
		g_doc = nil;
	}
    
	VUPRINTF("iCloud: Destroying write data\n");
    
	if (g_data.m_ToWriteData)
	{
		free(g_data.m_ToWriteData);
		g_data.m_ToWriteData = NULL;
	}
    
    VUPRINTF("iCloud: Clean up finished\n");
}

void IOSiCloudUpdateWriting()
{
	if (!g_data.m_ReadyForWriting || g_data.m_WriteInProgress || !g_data.m_ScheduledWriting)
		return;
	
	g_data.m_ScheduledWriting = false;
	g_data.m_WriteInProgress = true;
	
	// Create document if not done before
	
	if (!g_doc)
	{
		VUPRINTF("iCloud: Pre-writing: need to create document\n");
		
		NSURL *ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
		NSString* fileNameString = [[NSString alloc] initWithUTF8String: g_data.m_Name];
        NSURL *ubiquitousPackage = [[ubiq URLByAppendingPathComponent:@"Documents"] URLByAppendingPathComponent:fileNameString];
// ARC enabled
// [fileNameString release];
		
		IOSiCloudCreateDoc(ubiquitousPackage);
		
		VUPRINTF("iCloud: Pre-writing: document created\n");
	}
	
	// Start writing the document
	
	VUPRINTF("iCloud: Starting write\n");
	
	[g_doc saveToURL:		[g_doc fileURL]
	forSaveOperation:	UIDocumentSaveForCreating
   completionHandler:	^(BOOL success)
	 {
		 g_data.m_WriteInProgress = false;
		 g_data.m_WriteSucceeded = success;
		 
		 VUPRINTF("iCloud: Write %s\n", success ? "succeeded" : "failed");
	 }
	 ];
}

void IOSiCloudUpdateReading(float fdt)
{
	if (!g_data.m_RetryRead)
		return;
    
	// Retry to read if failed last time
	
	g_data.m_RetryReadTimer += fdt;
	if (g_data.m_RetryReadTimer < 10.0f) // Wait X seconds until next try
		return;
	g_data.m_RetryRead = false;
	g_data.m_RetryReadTimer = 0.0f;
	
	IOSiCloudStartReading();
}

//*****************************************************************************
void VuIosCloudSaveManagerObjC::tick(float fdt)
{
	if (!g_data.m_IsAvailable)
		return;
	
	IOSiCloudUpdateReading(fdt);
	IOSiCloudUpdateWriting();
}

//*****************************************************************************
bool VuIosCloudSaveManagerObjC::write(const void *data, int dataSize)
{
	if (!g_data.m_IsAvailable)
		return false;
	
	// Read or verify that the document doesn't exist yet first
	
	if (!g_data.m_ReadyForWriting)
	{
		VUPRINTF("iCloud: Failed to write, reason: query or read operation didn't finish yet\n");
		return false;
	}
	
	// Don't break current write
	
	if (g_data.m_WriteInProgress)
	{
		VUPRINTF("iCloud: Failed to write, reason: previous write in progress\n");
		return false;
	}
	
	// Store data to write
	
	if (g_data.m_ToWriteDataSize != dataSize)
	{
		if (g_data.m_ToWriteData)
		{
			free(g_data.m_ToWriteData);
			g_data.m_ToWriteDataSize = 0;
		}
		
		g_data.m_ToWriteData = malloc(dataSize);
		if (!g_data.m_ToWriteData)
        {
			VUPRINTF("iCloud: Failed to write, reason: failed to allocate %d bytes for temporary buffer\n", dataSize);
			return false;
		}
        g_data.m_ToWriteDataSize = dataSize;
	}
	
	memcpy(g_data.m_ToWriteData, data, dataSize);
	
	// Write / schedule write
	
	g_data.m_ScheduledWriting = true;
	VUPRINTF("iCloud: Write scheduled\n");
	
	IOSiCloudUpdateWriting();
	
	return true;
}
