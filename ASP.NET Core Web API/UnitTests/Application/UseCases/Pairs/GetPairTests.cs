using Application.UseCases.Pairs.GetPair;
using Domain.Entities;
using Domain.RepositoryInterfaces;

namespace UnitTests.Application.UseCases.Pairs;

[TestFixture]
public class GetPairTests
{
    private GetPairHandler _getPairHandler = null!;
    private readonly Mock<IPairRepository> _fakePairRepository = new();

    [OneTimeSetUp]
    public void OneTimeSetUp()
    {
        _getPairHandler = new GetPairHandler(_fakePairRepository.Object);
    }

    [SetUp]
    public void Setup()
    {
        _fakePairRepository.Invocations.Clear();
    }

    [TestFixture]
    public class HandleMethod: GetPairTests
    {
        [TestCase(true)]
        [TestCase(false)]
        public async Task InputIsValid_ReturnsResponse(bool doesEntityExistInDb)
        {
            // Arrange
            var request = new GetPairRequest(1);

            var fakeDbData = new Pair {Id = 1, Name = "Example_Pair_Name_1", Value = "Example_Pair_Value_1"};
            
            _fakePairRepository
                .Setup(x => x.GetAsync(request.Id, It.IsAny<CancellationToken>()))
                .ReturnsAsync(doesEntityExistInDb ? fakeDbData : null);

            var expected = new GetPairResponse(doesEntityExistInDb ? fakeDbData : null);

            // Act
            var actual = await _getPairHandler.Handle(request, default);

            // Assert
            if (doesEntityExistInDb) 
                Assert.IsNotNull(actual.Pair);
            else 
                Assert.IsNull(actual.Pair);

            Assert.AreEqual(expected.Pair, actual.Pair);
        }
        
        [TestCase(0)]
        [TestCase(-1)]
        public void InputIsNotValid_ThrowsException(int id)
        {
            // Arrange
            var request = new GetPairRequest(id); // Id must be positive

            // Act and Assert
            Assert.ThrowsAsync<ArgumentException>(async () => await _getPairHandler.Handle(request, default));
            _fakePairRepository.VerifyNoOtherCalls();
        }
    }
}